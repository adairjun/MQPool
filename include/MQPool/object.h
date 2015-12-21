#ifndef MQPOOL_INCLUDE_OBJECT_H_
#define MQPOOL_INCLUDE_OBJECT_H_

// 最原始的抽象class,仅仅是为了实现classFactory的模式
class Object {
 public:
  Object();
  virtual ~Object();
  virtual void Dump() const = 0;
};

#endif /* MQPOOL_INCLUDE_OBJECT_H_ */
