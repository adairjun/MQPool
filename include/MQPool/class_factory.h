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
 * 这里返回一个空指针的原因是为了能将函数指针统一放入factoryMap_当中
 * 我在考虑使用boost::any来代替这里的void*,或者使用boost::function来代替这里的函数指针
 * Effective C++提出尽量少做转型操作，用static_cast这种新式转型来代替旧式转型
 */
typedef void* (*ObjectCreate_t)();

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

  /*
   * 关于为什么要把这里的返回值设置成为bool而不是void，原因是
   * 下面需要用到宏IMPL_CLASS_CREATE
   * 如果返回值是void的话，宏当中的AddObject就只剩这样的了：
   * ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name)
   * 就没有前面的把返回值赋值给_bUnused的用法了
   * 倒不是说这个_Unused有多重要，看我给它起的名字是unused就知道它没有用了，是因为这个宏的使用一定是在函数的外部使用
   * 这样一来简单的调用AddObject就会有错：ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name)
   * 因为C++不允许在全局的作用域当中调用函数。而写成这种形式：
   * static bool _##class_name##_Unused __attribute__((unused))= ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name);
   * 这个__attribute__((unused))是编译器的内置宏，就是告诉编译器当这个函数没有被使用到的时候不要抛出警告，因为C++的规则就是定义没有调用的函数是有警告的
   * 所以上面那条语句其实等价于
   * static bool _Unused = ClassFactory::Instance().AddObject(#class_name, CreateClass##class_name);
   * 这就是初始化一个全局变量了，C++就判断合法
   * 所以下面的宏的用法包括这里的返回值为bool而不是void的用法就是这么个意思
   */
  bool AddObject(const string& className, ObjectCreate_t pCreate);
  bool AddObject(const char* className, ObjectCreate_t pCreate);
  void* GetObject(const string& className);
  void* GetObject(const char* className);

  map<string, ObjectCreate_t> GetMap() const;

 private:
  map<string, ObjectCreate_t> factoryMap_;
};

/*
 * Effective C++的告诫：必须返回对象的时候，不要返回reference,
 * 因为任何时候看到一个reference声明式，都应该立刻问自己：它的另外一个名字是什么?
 * 因为它一定是某物的另一个名称。
 * Effective C++特地指明了当返回一个local static对象的reference的时候，要确保不能同时需要多个这种对象
 * 这在单线程当中很容易实现，确保了这种对象不同时需要的话，就可以使用条款4的singleton手法
 * 但是在多线程环境当中可不能用，任何一种non-const static对象，不论它是local还是non-local，在多线程环境当中等待某事发生都会有麻烦
 * 为了避免这种麻烦，这个函数应该用在程序的单线程启动阶段，就是在多线程还没启动的时候就先调用了，这就可以消除与初始化有关的race conditions
 * 根据条款4，把这个singleton设置为inline
 */
inline ClassFactory& ClassFactoryInstance() {
  static ClassFactory sInstance;
  return sInstance;
}

#define DECLARE_CLASS_CREATE(class_name)	    	\
	static void* CreateClass##class_name ();

#define IMPL_CLASS_CREATE(class_name)	            \
	static void* CreateClass##class_name (){	    \
		return (void*)(new class_name());			\
	};											    \
	static bool _##class_name##_Unused __attribute__((unused))= ClassFactoryInstance().AddObject(#class_name, CreateClass##class_name);

//#的作用是在class_name的左右两边都加上双引号，##的作用是连接两个字符串

#endif /* MQPOOL_INCLUDE_CLASSS_FACTORY_H_ */
