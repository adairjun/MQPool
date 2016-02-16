#include <iostream>
#include <string.h>

#include "MQueue/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace rapidmsg;

/*
 * 使用共享内存来做消息队列的任务
 */

int main() {

  /*
   * 初始化存放消息的共享内存，即主内存
   */
  ShmAllocator shmAllocator(true);
  shmAllocator.Attach();
  shmAllocator.InitPHead();

  /* 初始化管理内存，用于管理主内存 */

  ManagerMem manager(true);
  manager.Attach();
  manager.InitPHead();

  while(1) {
	long messageId = test::JUST_TEST_REQUEST;

	// 我这里保证只有这一个进程使用JUST_TEST_REQUEST这个ID，所以不用加锁, shmAllocator也是一样
	// 但是如果有多个进程，那就必须加锁
	uint64_t offset = manager.GetOffsetById(messageId);

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
      cout << "rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_id()"
      		<< rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_id() << endl;
      cout << "rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_name()"
    		<< rmessage.body().GetExtension(::rapidmsg::test::just_test_request).test_name() << endl;

      messageId = test::JUST_TEST_RESPONSE;
      RMessage pmessage;

      Head* head = new Head;
      head->set_session_no("1");
      head->set_message_type(messageId);
      head->set_client_ip("127.0.0.1");
      head->set_target_ip("127.0.0.1");
      head->set_target_port(9999);

      Body* body = new Body;

      ResponseCode* responseCode = new ResponseCode;
      responseCode->set_retcode(0);
      responseCode->set_error_message("");

      ::rapidmsg::test::JustTestResponse* response = body->MutableExtension(::rapidmsg::test::just_test_response);

      response->set_test_id(1);
      response->set_allocated_rc(responseCode);

      pmessage.set_allocated_head(head);
      pmessage.set_allocated_body(body);

      string str;
      pmessage.SerializeToString(&str); // 将对象序列化到字符串

      void* pt = shmAllocator.Allocate(str.length(), offset);
      memcpy(pt, str.c_str(), str.length());

      // 将offset和test::JUST_TEST_RESPONSE建立映射关系
      manager.AddIdOffsetMapping(test::JUST_TEST_RESPONSE, offset);
	}
  }
  return 0;
}
