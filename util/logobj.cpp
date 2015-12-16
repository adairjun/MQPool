#include "../include/MQPool/logobj.h"
#include <time.h>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
  LogObj::LogLevel eLevel;
  string pName;
} LevelName;


// 这是表驱动法
static LevelName LEVEL_NAME_MAPPING[] = {
  {LogObj::TRACE, "TRACE"},
  {LogObj::DEBUG, "DEBUG"},
  {LogObj::INFO, "INFO"},
  {LogObj::WARN, "WARN"},
  {LogObj::ERROR, "ERROR"},
  {LogObj::FATAL, "FATAL"},
  {LogObj::NUM_LOG_LEVELS, "NUM_LOG_LEVELS"}
};


LogObj::LogLevel LogObj::LogLevelConvert(string pLevel) {
  for (unsigned i = 0;i < sizeof(LEVEL_NAME_MAPPING)/sizeof(LevelName); ++i) {
  if ( LEVEL_NAME_MAPPING[i].pName == pLevel) {
	  return LEVEL_NAME_MAPPING[i].eLevel;
	}
  }
  return LogObj::NUM_LOG_LEVELS;
}

string LogObj::LogLevelConvert(LogObj::LogLevel eLevel) {
  for (unsigned i = 0; i < sizeof(LEVEL_NAME_MAPPING)/sizeof(LevelName); ++i) {
	if ( LEVEL_NAME_MAPPING[i].eLevel == eLevel ) {
	  return LEVEL_NAME_MAPPING[i].pName;
	}
  }
  return "";
}

LogObj::LogObj()
    : prefix_("undefined_"),
	  suffix_(".log"),
	  savePath_("../../log/MQPool/"),
	  maxSize_(100ul * 1024ul * 1024ul),
	  maxLogSize_(20ul * 1024ul * 1024ul * 1024ul),
	  logLevel_(DEBUG),
	  lineBufferSize_(1024),
	  fd_(-1),
	  currentDay_(""),
	  currentFile_(""){
}

LogObj::~LogObj() {
  if ( fd_ != -1 ) {
	close(fd_);
	fd_ = -1;
  }
}

void LogObj::Dump() const {
  printf("\n=====LogObj Dump START ========== \n");
  printf("prefix_=%s", prefix_.c_str());
  printf("suffix_=%s", suffix_.c_str());
  printf("savePath_=%s", savePath_.c_str());
  printf("maxSize_=%lld", maxSize_);
  printf("maxLogSize_=%lld", maxLogSize_);
  printf("LogLevel_=%s", LogLevelConvert(logLevel_).c_str());
  printf("lineBufferSize_=%d", lineBufferSize_);
  printf("fd_=%d", fd_);
  printf("\n===LogObj DUMP END ============\n");
}

string LogObj::GetPrefix() const {
  return prefix_;
}

string LogObj::GetSuffix() const {
  return suffix_;
}

unsigned long long LogObj::GetMaxSize() const {
  return maxSize_;
}

unsigned long long LogObj::GetMaxLogSize() const {
  return maxLogSize_;
}

LogObj::LogLevel LogObj::GetLogLevel() const {
  return logLevel_;
}

string LogObj::GetCurrentDay() const {
  return currentDay_;
}

string LogObj::GetCurrentFile() const {
  return currentFile_;
}

unsigned LogObj::GetLineBufferSize() const {
  return lineBufferSize_;
}

int LogObj::GetFd() const {
  return fd_;
}

void LogObj::SetPrefix(const string& prefix) {
  prefix_ = prefix;
}

void LogObj::SetSuffix(const string& suffix) {
  suffix_ = suffix;
}

void LogObj::SetSavePath(const string& savePath) {
  savePath_ = savePath;
}

void LogObj::SetMaxSize(unsigned long long maxSize) {
  maxSize_ = maxSize;
}

void LogObj::SetMaxLogSize(unsigned long long maxLogSize) {
  maxLogSize_ = maxLogSize;
}

void LogObj::SetLogLevel(const string& logLevel) {
  logLevel_ = LogLevelConvert(logLevel);
}

void LogObj::SetLineBufferSize(unsigned lineBufferSize) {
  lineBufferSize_ = lineBufferSize;
}

string LogObj::GetCurrTime(int flag) const {
  string format = "%Y-%m-%d %H:%M:%S";
  // 如果flag设为0的话，那么旧仅仅获取当前的日期，后面的小时，分钟，秒都不用
  if (flag == 0){
	  string format = "%Y%m%d";
  }
  time_t now;
  time(&now);
  struct tm nowtm;
  localtime_r(&now, &nowtm);

  char ftime[30];
  size_t len = strftime((char *)&ftime, sizeof(ftime)-1, format.c_str(), &nowtm);
  return std::string((char *)&ftime, len+1); //len+1 to include the last '\0' character
}

void LogObj::CheckFile() {
  //检查文件是否存在，如果不存在那么就关闭文件描述符号
  //access得到当前用户是否有操作这个文件的权限，F_OK表示文件存在，R_OK表示有读权限,W_OK表示有写权限，X_OK表示有执行权限
  if (fd_ != -1 && access(currentFile_.c_str(), F_OK)!=0) {
     close(fd_);
     fd_ = -1;
     currentFile_ = "";
  }

  //检查单个日志的大小是不是已经超过了限制
  //如果超过了限制那么就关闭文件描述符，禁止写入
  if (fd_ != -1) {
	struct stat64 sStat;
	fstat64(fd_, &sStat);
	if ((unsigned long long)sStat.st_size > maxSize_ ) {
	  close(fd_);
	  fd_ = -1;
	}
  }

  //这个时候如果fd_有值的话，就不必再进行下面的判断了，直接return
  if (fd_ != -1) {
	return;
  }

  currentDay_ = GetCurrTime(0);

  //检查日志的总大小是否已经超过了限制
  //这个很好判断，我打日志的策略是这样的：
  //日志名称是prefix_ 加上 currentDay_ 加上 iIndex 加上 suffix_
  //这里的iIndex是为了应付一个日志文件超过了限制值的情况，那么就自增iIndex，也就是往下一个日志当中写入
  //这里的日志总大小当然是一天以内的日志，因为日志的名字都是用currentDay_，是用天表示
  //如果超过了那么就删除旧日志
  int iIndex = 0;
  unsigned long long iTotalSize = 0;
  char fileName[128];
  while (true) {
	//snprintf的作用就是把"%s/%s%s%03d%s"之后的内容格式化之后赋值给szFileName
    snprintf(fileName, sizeof(fileName), "%s/%s%s%03d%s", savePath_.c_str(), prefix_.c_str(), currentDay_.c_str(), iIndex, suffix_.c_str());
    struct stat64 sStat;
    if ( 0 != lstat64(fileName, &sStat) ) {
      break;
    }
    // 计算总计的日志大小
    iTotalSize += sStat.st_size;
    if ((unsigned long long)sStat.st_size > maxSize_) {
      iIndex++;
      continue;
    }
    //如果能执行到这里就说明没有执行上面的continue,也就说明了读到的日志没有写满，那就一定是最后的日志
    break;
  }
  if (iTotalSize >= maxLogSize_) {
	//TODO delete old file
  }

  currentFile_ = fileName;
  fd_ = open(fileName, O_CREAT|O_RDWR|O_APPEND|O_LARGEFILE, 00640);
}

void LogObj::MakeLog(LogObj::LogLevel logLevel, const char *pFormat, va_list sAp) {
  if (logLevel_ <  logLevel) {
	return;
  }
  //在打日志的时候，需要先做检查
  CheckFile();

  if (fd_ == -1) {
	return;
  }

  //开始把日志消息写到文件中
  char *pLineBuffer = (char *)alloca(lineBufferSize_);
  unsigned iOffset = 0;
  //encode time and log level
  iOffset = snprintf(pLineBuffer+iOffset, lineBufferSize_, "%s[%s] ", GetCurrTime(1).c_str(), LogLevelConvert(logLevel_).c_str());
  //encode message
  iOffset += vsnprintf(pLineBuffer+iOffset, lineBufferSize_-iOffset, pFormat, sAp);
  //print message and newline
  dprintf(fd_, "%s\n", pLineBuffer);
}
