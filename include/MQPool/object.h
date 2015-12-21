#ifndef MQPOOL_INCLUDE_OBJECT_H_
#define MQPOOL_INCLUDE_OBJECT_H_

class Object {
 public:
  Object();
  virtual ~Object();
  virtual void Dump() const = 0;
};

#endif /* MQPOOL_INCLUDE_OBJECT_H_ */
