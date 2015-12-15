#ifndef MQPOOL_INCLUDE_LOGOBJ_H_
#define MQPOOL_INCLUDE_LOGOBJ_H_

#include <string>

using std::string;

class LogObj {
 public:

  typedef enum {
    TRACE,            //跟踪
	DEBUG,            //除错
	INFO,             //信息
	WARN,             //警告
	ERROR,            //错误
	FATAL,            //致命
	NUM_LOG_LEVELS    //日志级别的数量
  } LogLevel;

  static LogLevel LogLevelConvert(string pLevel);
  static string LogLevelConvert(LogLevel eLevel);

 public:

  explicit LogObj();
  virtual ~LogObj();

  void Dump() const;

  string GetPrefix() const;

  string GetSuffix() const;

  unsigned long long GetMaxSize() const;

  unsigned long long GetMaxLogSize() const;

  LogLevel GetLogLevel() const;

  string GetCurrentDay() const;

  string GetCurrentFile() const;

  unsigned GetLineBufferSize() const;

  int GetFd() const;

  //======================================================================

  void SetPrefix(const string& prefix);

  void SetSuffix(const string& suffix);

  void SetSavePath(const string& savePath);

  void SetMaxSize(unsigned long long maxSize);

  void SetMaxLogSize(unsigned long long maxLogSize);

  // 对LogLevel的设置就直接传string
  void SetLogLevel(const string& logLevel);

  void SetLineBufferSize(unsigned lineBufferSize);

 private:
  string prefix_;                         //日志文件名，这里以模块名称来命名
  string suffix_;                         //日志文件名，后缀统一为：.log
  string savePath_;                       //日志保存目录
  unsigned long long maxSize_;            //单个日志文件的最大尺寸
  unsigned long long maxLogSize_;         //日志总计的最大尺寸
  LogLevel logLevel_;                     //日志级别

  unsigned lineBufferSize_;

  string currentDay_;                     //相比glog的默认时间单位，我这里直接使用天作为时间单位
  string currentFile_;                    //当前文件名

  int fd_;                                //日志文件的文件描述符号
};


#endif /* MQPOOL_INCLUDE_LOGOBJ_H */
