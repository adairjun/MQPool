#ifndef MQPOOL_INCLUDE_LOGSTREAM_H
#define MQPOOL_INCLUDE_LOGSTREAM_H

#include <string>

using std::string;

class LogStream {
 public:
  explicit LogStream();
  LogStream(const LogStream&);
  LogStream& operator=(const LogStream&);
  virtual ~LogStream();

  void Dump() const;

 public:
 /* LogStream& operator<<(bool);

  LogStream& operator<<(short);
  LogStream& operator<<(unsigned short);
  LogStream& operator<<(int);
  LogStream& operator<<(unsigned int);
  LogStream& operator<<(long);
  LogStream& operator<<(unsigned long);
  LogStream& operator<<(long long);
  LogStream& operator<<(unsigned long long);

  LogStream& operator<<(float);
  LogStream& operator<<(double);*/
  LogStream& operator<<(char);
  LogStream& operator<<(const char*);

  LogStream& operator<<(const string&);

 public:
  void Append(const char* buf, unsigned int len);

  const char* GetBuffer();

  void ResetBuffer();

  template<typename T>
    void formatInteger(T);

 private:
  char buffer_[4096];

  static const int kMaxNumericSize = 32;

};

#endif /* MQPOOL_INCLUDE_LOGSTREAM_H */
