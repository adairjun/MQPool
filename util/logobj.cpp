#include "MQPool/logobj.h"

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

static LogLevel LogLevelConvert(string pLevel);

LogObj::LogLevel LogObj::LogLevelConvert(string pLevel) {
  for (unsigned i = 0;i < sizeof(LEVEL_NAME_MAPPING)/sizeof(LevelName); ++i) {
	if ( strcasecmp(LEVEL_NAME_MAPPING[i].pName, pLevel) == 0 ) {
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

LogObj::Log()
    : prefix_("undefined_"),
	  suffix_(".log"),
	  savePath_("/var/log/MQPool/"),
	  maxSize_(100ul * 1024ul * 1024ul),
	  maxLogSize_(20ul * 1024ul * 1024ul * 1024ul),
	  lineBufferSize_(1024),
	  fd_(-1) {
}

LogObj::~Log() {
  if (fd_ != -1 ) {
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

void LogObj::SetLineBufferSize(unsigned lineBufferSize) {
  lineBufferSize_ = lineBufferSize;
}

static LogStream LogObj::MakeLogByStream(const char* file, int line, LogLevel logLevel) {
  return stream_;
}
