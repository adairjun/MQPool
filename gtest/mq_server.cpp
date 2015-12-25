#include <iostream>
#include <string.h>  

#include "MQPool/message_queue.h"
#include "MQPool/parse_xml.h"
#include <boost/lexical_cast.hpp>
using namespace std;

/*
 * 消息队列与protobuf结合起来使用方能凸显威力
 */

int main()  
{  
	MessageQueue myQueue("key");
	ParseXmlObj myXmlObj("../config/messageID.xml");
	struct myMsg msg;

	while(1)  
	{
		long messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_REQUEST", "value"));
		myQueue.RecvMsg(messageId, &msg);

		MessageFactory::MessageType messageType_;
		string sendServiceName;
		string message;
		MessageFactoryInstance().ParseMyMsg(msg, messageId, messageType_, sendServiceName, message);


        cout << "sendServiceName--------" << sendServiceName << endl;
        cout << "message--------" << message << endl;

        messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_RESPONSE", "value"));
        msg = MessageFactoryInstance().CreateMyMsg(messageId, MessageFactory::RESPONSE, "mq_server", "THIS IS RESPONSE");
		myQueue.SendMsg(&msg);
	}  
	exit(0);  
}
