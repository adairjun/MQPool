#ifndef MQPOOL_INCLUDE_CLASS_FACTORY_H_
#define MQPOOL_INCLUDE_CLASS_FACTORY_H_

#include "object.h"
#include <string>
#include <map>

using std::string;
using std::map;
using std::make_pair;

/*
 * 使用这个typedef的前提是需要Object的派生类实现自己的无参构造函数
 * 如果没有无参构造函数的话当然是不能成功的
 */
typedef Object *(*ObjectCreate_t)();
/*
 * 使用factory模式来模拟JAVA的反射机制
 */
class ClassFactory : public Object {
 public:
  ClassFactory();
  ClassFactory(const ClassFactory&) = delete;
  ClassFactory& operator=(const ClassFactory&) = delete;
  virtual ~ClassFactory();
  void Dump() const;

 public:
  static ClassFactory& Instance();
  /*
   * 关于为什么要把这里的返回值设置成为bool而不是void，原因是
   * 下面需要用到宏IMPL_CLASS_CREATE
   * 如果返回值是void的话，宏当中的AddObject就只剩这样的了：
   * ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name)
   * 就没有前面的把返回值赋值给_bUnused的用法了
   * 倒不是说这个_bUnused有多重要，看我给它起的名字是unused就知道它没有用了，是因为这个宏的使用一定是在函数的外部使用
   * 这样一来简单的调用AddObject就会有错：ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name)
   * 因为C++不允许这样做
   * 但是赋值就不一样，赋值可以直接在函数外部赋值，所以下面的宏的用法包括这里的返回值为bool而不是void的用法就是这么个意思
   */
  bool AddObject(const string& className, ObjectCreate_t pCreate);
  bool AddObject(const char* className, ObjectCreate_t pCreate);
  Object* GetObject(const string& className);
  Object* GetObject(const char* className);

  map<string, ObjectCreate_t> GetMap() const;

 private:
  map<string, ObjectCreate_t> factoryMap_;
};

#define DECLARE_CLASS_CREATE(class_name)	    	\
	static Object* CreateClass##class_name ();

#define IMPL_CLASS_CREATE(class_name)	            \
	static Object* CreateClass##class_name (){	    \
		return new class_name();			        \
	};											    \
	static bool _bUnused __attribute__((unused))= ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name);

//#的作用是在class_name的左右两边都加上双引号，##的作用是连接两个字符串

#endif /* MQPOOL_INCLUDE_CLASSS_FACTORY_H_ */
