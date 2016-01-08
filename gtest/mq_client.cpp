#include <iostream>

#include "MQPool/message_queue.h"
/*#include "MQPool/parse_xml.h"*/
#include "rapidmsg.pb.h"
#include "test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

#include <string.h>

using namespace std;
using namespace rapidmsg;

#define PERM S_IRUSR|S_IWUSR  

int main(int argc,char **argv)  
{  
	MessageQueue myQueue("key");
	/*ParseXmlObj myXmlObj("../config/messageID.xml");
	long messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_REQUEST", "value"));  //根据CREATE_WORKFLOW_REQUEST得到1
	struct myMsg msg = MessageFactory::Instance().CreateMyMsg(messageId, MessageFactory::REQUEST, "mq_client", "THIS IS REQUEST");
*/
	long messageId = test::JUST_TEST_REQUEST;

	RMessage rmessage;

	Head head;
	head.set_session_no("1");
	head.set_message_type(messageId);
	head.set_client_ip("127.0.0.1");
	head.set_target_ip("127.0.0.1");
	head.set_target_port(9999);

	Body body;

	::rapidmsg::test::JustTestRequest* request = body.MutableExtension(::rapidmsg::test::just_test_request);
	request->set_test_id(1);
	request->set_test_name("test_request");

	rmessage.set_allocated_head(&head);
	rmessage.set_allocated_body(&body);


	string str;
	rmessage.SerializeToString(&str); // 将对象序列化到字符串，除此外还可以序列化到fstream等

	struct rapidMsg msg = MessageFactory::Instance().CreateRapidMsg(messageId,str);
	myQueue.SendMsg(&msg);

	memset(&msg, 0, sizeof(struct rapidMsg));

	messageId = test::JUST_TEST_RESPONSE;

	/*messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_RESPONSE", "value"));  //根据CREATE_WORKFLOW_RESPONSE得到2*/
	myQueue.RecvMsg(2, &msg);

	/*MessageFactory::MessageType messageType_;*/
	/*string sendServiceName;*/
	string message;
	MessageFactory::Instance().ParseRapidMsg(msg, messageId, message);

	rmessage.ParseFromString(message);

	/*cout << "sendServiceName--------" << sendServiceName << endl;
	cout << "message--------" << message << endl;
*/
	return 0;
}
