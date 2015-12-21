#ifndef MQPOOL_INCLUDE_CLASS_FACTORY_H_
#define MQPOOL_INCLUDE_CLASS_FACTORY_H_

#include "object.h"
#include <string>
#include <map>

using std::string;
using std::map;

typedef Object *(*ObjectCreate_t)();

class ClassFactory {
 public:
  ClassFactory();
  ClassFactory(const ClassFactory&) = delete;
  ClassFactory& operator=(const ClassFactory&) = delete;
  virtual ~ClassFactory();
  void Dump() const;

 public:
  static ClassFactory& Instance();
  bool AddObject(int iId, ObjectCreate_t pCreate);
  bool AddObject(const char *pId, ObjectCreate_t pCreate);
  Object *NewObject(int iId);
  Object *NewObject(const char *pId);

  map<string, ObjectCreate_t> GetMap() const {
  	return mapFactory_;
  }

 private:
  map<string, ObjectCreate_t> mapFactory_;
};

#define DECLARE_CLASS_CREATE(class_name)	/static Object* CreateClass##class_name ();

#define IMPL_CLASS_CREATE(class_name)	/static Object* CreateClass##class_name (){	/return new class_name();             /};

#define REG_CLASS_CREATE(Id, ObjectCreatePoint)     /ClassFactory::Instance().AddObject(Id, ObjectCreatePoint);

#endif /* MQPOOL_INCLUDE_CLASSS_FACTORY_H_ */
