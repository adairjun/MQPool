#include <iostream>
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include "rapidmsg/rmessage_util.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/generated_message_reflection.h>

#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace rapidmsg;

int main(int argc, char** argv) {
	/*string type_name = ::rapidmsg::test::JustTestRequest::descriptor()->full_name();
	cout << "type_name :----" << type_name << endl;

	const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);

	cout << "FindMessageTypeByName() = " << descriptor << endl;
	cout << "::rapidmsg::test::JustTestRequest::descriptor()  = " << ::rapidmsg::test::JustTestRequest::descriptor() << endl;

	const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);

	::rapidmsg::test::JustTestRequest* new_obj = dynamic_cast<::rapidmsg::test::JustTestRequest*>(prototype->New());

	delete new_obj;*/

	/*boost::shared_ptr<::rapidmsg::RMessage> pMessage = NewMessage("first", ::test::JUST_TEST_REQUEST);
	cout << pMessage->head().session_no() << endl;
	cout << pMessage->head().message_type() << endl;*/

	/*boost::shared_ptr<::google::protobuf::Message> reflmessage = CreateMessage("rapidmsg.test.JustTestRequest");*/


	/*RMessage rmessage;

	Head* head = new Head;
	head->set_session_no("1");
	head->set_message_type(::test::JUST_TEST_REQUEST);
	head->set_client_ip("127.0.0.1");
	head->set_target_ip("127.0.0.1");
	head->set_target_port(9999);

	Body* body = new Body;
	::rapidmsg::test::JustTestRequest* request = body->MutableExtension(::rapidmsg::test::just_test_request);
	request->set_test_id(1);
	request->set_test_name("test_request");

	rmessage.set_allocated_head(head);
	rmessage.set_allocated_body(body);

    RMessage response;
    MakeResponse(rmessage, ::test::JUST_TEST_RESPONSE, response);

    cout << response.head().session_no() << endl;
    cout << response.head().message_type() << endl;
	cout << response.head().client_ip() << endl;
    cout << response.head().client_port() << endl;
    cout << response.head().target_ip() << endl;

    cout << DebugString(rmessage) << endl;
    cout << DebugString(response) << endl;*/

	/*IpAddress myip;
	myip.set_ip("1");

	const ::google::protobuf::Reflection* reflection = myip.GetReflection();
    const ::google::protobuf::FieldDescriptor *field = NULL;
    const ::google::protobuf::Descriptor *descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("rapidmsg.IpAddress");

    field = descriptor->FindFieldByName("type");
    reflection->SetUInt32(&myip, field, 12);

    cout << myip.DebugString() << endl;*/

    //================================================================================

    /*RMessage rmes;
    Head* head = new Head;
    head->set_session_no("1");
    head->set_message_type(::test::JUST_TEST_REQUEST);
    head->set_client_ip("127.0.0.1");
    head->set_client_port(6666);
    head->set_target_ip("127.0.0.2");
    head->set_target_port(9999);

    Body* body = new Body;
    ::rapidmsg::test::JustTestRequest* request = body->MutableExtension(::rapidmsg::test::just_test_request);
    request->set_test_id(1);
    request->set_test_name("test_request");

    rmes.set_allocated_head(head);
    rmes.set_allocated_body(body);

    const ::google::protobuf::Reflection* rmes_ref = rmes.GetReflection();
    const ::google::protobuf::FieldDescriptor* rmes_field = NULL;
    const ::google::protobuf::Descriptor* rmes_descriptor = ::google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName("rapid.RMessage");
    rmes_field = rmes_descriptor->FindFieldByName("body");*/
    /*rmes_ref->SetUInt32(&rmes, rmes_field, 1000);*/

    /*cout << rmes.DebugString() << endl;*/


	string type_name = "rapidmsg.IpAddress";
	IpAddress myip;
	myip.set_ip("20");
	myip.set_type(10);
	string str;
	myip.SerializeToString(&str);

	/*
	string type_name = "rapidmsg.RMessage";
	::rapidmsg::RMessage rmessage;

	Head* head = new Head;
	head->set_session_no("1");
	head->set_message_type(::test::JUST_TEST_REQUEST);
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
	rmessage.SerializeToString(&str);
*/
    string send_mes = type_name + "###" + str;   //###做为分隔符

    //接下来就是将send_mes发送出去，为了方便我就不写了，下面的代码就是解析send_mes，就可以用在服务器端

    vector<string> fields;
    boost::split(fields, send_mes, boost::is_any_of("###"));  //将send_mes根据###切割，放入vector<string>当中去
    string recv_type_name("");
    string recv_message("");
    for (int i = 0; i<fields.size(); ++i) {
    	recv_type_name = fields[0];
    	if (i!=0 && fields[i]!="") {
    		recv_message = fields[i];
    	}
    }

   boost::shared_ptr<::google::protobuf::Message> new_obj = ParseFromStr(recv_type_name, recv_message);

   string sss = GetStringFieldByPointer(new_obj, "ip", 0);
   cout << "sss ------ " << sss << endl;

  return 0;
}
