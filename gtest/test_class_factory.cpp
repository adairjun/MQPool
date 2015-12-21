#include <iostream>
#include "MQPool/logobj.h"
#include "MQPool/parse_xml.h"
#include "MQPool/class_factory.h"

using namespace std;

//由于logobj没有实现自己的无参构造函数,所以这里的话我用parse_xml来做示范
DECLARE_CLASS_CREATE(ParseXmlObj);
IMPL_CLASS_CREATE(ParseXmlObj);

int main() {
  ClassFactory::Instance().Dump();
  Object* myxml = ClassFactory::Instance().GetObject("ParseXmlObj");
  //仅仅是利用C++的多态来模拟反射机制,但是多态下的指针只能调用虚函数,这个还没有完成
  myxml->Dump();

  delete myxml;

  return 0;
}
