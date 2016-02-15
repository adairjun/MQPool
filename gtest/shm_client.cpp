#include <iostream>
#include "MQPool/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

#include <string.h>

using namespace std;
using namespace rapidmsg;

int main(int argc, char** argv) {
  ShmAllocator shmAllocator(false);
  shmAllocator.Attach();

  ManagerMem manager(false);
  manager.Attach();

  long messageId = test::JUST_TEST_REQUEST;

  RMessage rmessage;

  Head* head = new Head;
  head->set_session_no("1");
  head->set_message_type(messageId);
  head->set_client_ip("127.0.0.1");
  head->set_target_ip("127.0.0.1");
  head->set_target_port(9999);

  Body* body = new Body;

  ::rapidmsg::test::JustTestRequest* request = body->MutableExtension(::rapidmsg::test::just_test_request);
  request->set_test_id(1);
  request->set_test_name("test_request");

  rmessage.set_allocated_head(head);
  rmessage.set_allocated_body(body);

  string str;
  rmessage.SerializeToString(&str); // 将对象序列化到字符串

  uint64_t offset;
  void* pt = shmAllocator.Allocate(str.length(), offset);
  memcpy(pt, str.c_str(), str.length());

  // 将offset和test::JUST_TEST_RESPONSE建立映射关系
  manager.AddIdOffsetMapping(test::JUST_TEST_REQUEST, offset);

  while(1) {
    //接收RESPONSE
    messageId = test::JUST_TEST_RESPONSE;

    offset = manager.GetOffsetById(messageId);
    if (offset != 0) {
	  uint64_t* psize = (uint64_t*)(shmAllocator.GetMemByOffset(offset-8));
	  uint64_t size = *psize;
	  char mess[size];
	  const void* vptr = shmAllocator.GetMemByOffset(offset);
	  memcpy(mess, vptr, size);
	  /*
	   * 获取到string类型的消息
	   */
	  string message(mess);

	  //反序列化
	  RMessage rmessage;
	  rmessage.ParseFromString(message);

	  // head
	  cout << "rmessage.head().session_no()" << rmessage.head().session_no() << endl;
	  cout << "rmessage.head().message_type()" << rmessage.head().message_type() << endl;
	  cout << "rmessage.head().client_ip()" << rmessage.head().client_ip() << endl;
	  cout << "rmessage.head().target_ip()" << rmessage.head().target_ip() << endl;
	  cout << "rmessage.head().target_port()" << rmessage.head().target_port() << endl;

	  //body
	  cout << "rmessage.body().GetExtension(::rapidmsg::test::just_test_response).test_id()"
	       << rmessage.body().GetExtension(::rapidmsg::test::just_test_response).test_id()
	       << endl;
    }
  }

  return 0;
}
