#include <iostream>
#include <string.h>  

#include "MQPool/message_queue.h"
/*#include "MQPool/parse_xml.h"*/
#include "rapidmsg.pb.h"
#include "test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace rapidmsg;

/*
 * 消息队列与protobuf结合起来使用方能凸显威力
 */

int main()  
{  
	MessageQueue myQueue("key");
	/*ParseXmlObj myXmlObj("../config/messageID.xml");*/
	struct rapidMsg msg;

	while(1)  
	{
		long messageId = test::JUST_TEST_RESPONSE;
		/*long messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_REQUEST", "value"));*/
		myQueue.RecvMsg(messageId, &msg);

		/*MessageFactory::MessageType messageType_;
		string sendServiceName;*/
		string message;
		MessageFactory::Instance().ParseRapidMsg(msg, messageId, message);


        /*cout << "sendServiceName--------" << sendServiceName << endl;
        cout << "message--------" << message << endl;*/

		messageId = test::JUST_TEST_RESPONSE;

        /*messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_RESPONSE", "value"));*/

		RMessage rmessage;

		Head head;
		head.set_session_no("1");
		head.set_message_type(messageId);
		head.set_client_ip("127.0.0.1");
		head.set_target_ip("127.0.0.1");
		head.set_target_port(9999);

		Body body;

		ResponseCode responseCode;
		responseCode.set_retcode(0);
		responseCode.set_error_message("");

		::rapidmsg::test::JustTestResponse* response = body.MutableExtension(::rapidmsg::test::just_test_response);

		response->set_test_id(1);
		response->set_allocated_rc(&responseCode);

		rmessage.set_allocated_head(&head);
		rmessage.set_allocated_body(&body);


		string str;
		rmessage.SerializeToString(&str); // 将对象序列化到字符串，除此外还可以序列化到fstream等


        msg = MessageFactory::Instance().CreateRapidMsg(messageId, str);
		myQueue.SendMsg(&msg);
	}  
	exit(0);  
}
