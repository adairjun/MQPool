#include "MQPool/logstream.h"
#include <string.h>
#include <stdio.h>

LogStream::LogStream() {
    memset(buffer_, 0, 4096);
}

LogStream::~LogStream() {
}

LogStream::LogStream(const LogStream& logStream) {
  this->ResetBuffer();
  /*const char buffer[4096] = logStream.GetBuffer();
  memcpy(this->buffer_, buffer, sizeof(buffer));*/
  memcpy(this->buffer_, "this is rrrrr", 13);
}

LogStream& LogStream::operator=(const LogStream& logStream) {
  this->ResetBuffer();
 /* const char buffer[4096] = logStream.GetBuffer();
  memcpy(this->buffer_, buffer, sizeof(buffer));*/
  memcpy(this->buffer_, "this is rrrrr", 13);
}

void LogStream::Dump() const {
  printf("\n=====LogStream Dump START ========== \n");
  printf("buffer_=%s", buffer_);
  printf("\n===LogStream DUMP END ============\n");
}

/*
LogStream& LogStream::operator <<(bool v) {
  Append(v ? "1" : "0", 1);
  return *this;
}

LogStream& LogStream::operator <<(short v) {
  *this << static_cast<int>(v);
  return *this;
}

LogStream& LogStream::operator <<(unsigned short v) {
  *this << static_cast<unsigned int>(v);
  return *this;
}

LogStream& LogStream::operator <<(int v) {
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator <<(unsigned int v) {
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator <<(long v) {
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator <<(unsigned long v) {
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator <<(long long v) {
  formatInteger(v);
  return *this;
}

LogStream& LogStream::operator <<(unsigned long long v) {
  formatInteger(v);
  return *this;
}


LogStream& LogStream::operator <<(float v) {
  *this << static_cast<double>(v);
  return *this;
}

LogStream& LogStream::operator <<(double v) {
  //先判断一下空间是否足够
	  if (sizeof(buffer_)-strlen(buffer_) >= kMaxNumericSize) {
		char array [kMaxNumericSize];
		//只保留三位小数
		sprintf(array, "%.3lf", v);
	    Append(array, sizeof(array));
	  }
	  return *this;
}
*/

LogStream& LogStream::operator <<(char v) {
  Append(&v, 1);
  return *this;
}

LogStream& LogStream::operator <<(const char* v) {
  printf("2222222222222");
  printf("%s", v);
  if (v) {
    Append(v, strlen(v));
    printf("33333333333\n");
    printf("%s", buffer_);
  } else {
    Append("(null)", 6);
  }
  return *this;
}

LogStream& LogStream::operator<<(const string& v) {
  Append(v.c_str(), v.size());
  return *this;
}

void LogStream::Append(const char* buf, unsigned int len) {
  if (strlen(buffer_) + len <= sizeof(buffer_)) {
    memcpy(buffer_ + strlen(buffer_), buf, len);
  } else {
    //TODO cerr<< "no enough space in buffer"
  }
}

const char* LogStream::GetBuffer() {
  return buffer_;
}

void LogStream::ResetBuffer() {
  memset(buffer_, 0, sizeof(buffer_));
}

template<typename T>
void LogStream::formatInteger(T v) {
  //先判断一下空间是否足够
  if (sizeof(buffer_)-strlen(buffer_) >= kMaxNumericSize) {
	char array [kMaxNumericSize];
	sprintf(array, "%d", v);
    Append(array, sizeof(array));
  }
}
