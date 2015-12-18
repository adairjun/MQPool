#ifndef MQPOOL_INCLUDE_LOGSTREAM_H
#define MQPOOL_INCLUDE_LOGSTREAM_H

class LogStream {
 public:
  LogStream();
  LogStream& (const LogStram&) = delete;
  LogStream& operator=(const LogStream&) = delete;
  virtual ~LogStream();

 public:
  LogStream& operator<<(bool);

  LogStream& operator<<(short);
  LogStream& operator<<(unsigned short);
  LogStream& operator<<(int);
  LogStream& operator<<(unsigned int);
  LogStream& operator<<(long);
  LogStream& operator<<(unsigned long);
  LogStream& operator<<(long long);
  LogStream& operator<<(unsigned long long);


  LogStream& operator<<(const void*);

  LogStream& operator<<(float);
  LogStream& operator<<(double);
  LogStream& operator<<(char);
  LogStream& operator<<(const char*);
  LogStream& operator<<(const unsigned char*);

  LogStream& operator<<(const string&);
  LogStream& operator<<(short);
  LogStream& operator<<(short);

  LogStream& operator<<(short);
  LogStream& operator<<(short);

 public:
  void append(const char* /*restrict*/ buf, size_t len) {
        memcpy(buffer_, buf, len);
    }
  const char* buffer() const {
	  return buffer_;
  }

  void resetBuffer() {

  }

 private:
  char buffer_[4096];

};

#endif /* MQPOOL_INCLUDE_LOGSTREAM_H */
