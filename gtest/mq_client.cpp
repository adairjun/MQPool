#include <iostream>

#include "MQPool/message_queue.h"
#include "MQPool/parse_xml.h"
#include <boost/lexical_cast.hpp>

#include <string.h>

using namespace std;

#define PERM S_IRUSR|S_IWUSR  

int main(int argc,char **argv)  
{  
	MessageQueue myQueue("key");
	ParseXmlObj myXmlObj("../config/messageID.xml");
	long messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_REQUEST", "value"));  //根据CREATE_WORKFLOW_REQUEST得到1
	struct myMsg msg = MessageFactory::Instance().CreateMyMsg(messageId, MessageFactory::REQUEST, "mq_client", "THIS IS REQUEST");

	myQueue.SendMsg(&msg);

	memset(&msg, 0, sizeof(struct myMsg));

	messageId = boost::lexical_cast<long>(myXmlObj.GetAttrByAttr("IDs", "id", "CREATE_WORKFLOW_RESPONSE", "value"));  //根据CREATE_WORKFLOW_RESPONSE得到2
	myQueue.RecvMsg(2, &msg);

	MessageFactory::MessageType messageType_;
	string sendServiceName;
	string message;
	MessageFactory::Instance().ParseMyMsg(msg, messageId, messageType_, sendServiceName, message);

	cout << "sendServiceName--------" << sendServiceName << endl;
	cout << "message--------" << message << endl;

	return 0;
}
