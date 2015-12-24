#include <iostream>

#include "MQPool/message_queue.h"
#include <string.h>

using namespace std;

#define PERM S_IRUSR|S_IWUSR  

int main(int argc,char **argv)  
{  
	MessageQueue myQueue("key");
	struct myMsg msg = MessageFactory::Instance().CreateMyMsg(1, MessageFactory::REQUEST, "mq_client", "THIS IS REQUEST");

	myQueue.SendMsg(&msg);

	memset(&msg, 0, sizeof(struct myMsg));

	myQueue.RecvMsg(2, &msg);

	long messageId;
	MessageFactory::MessageType messageType_;
	string sendServiceName;
	string message;
	MessageFactory::Instance().ParseMyMsg(msg, messageId, messageType_, sendServiceName, message);

	cout << "sendServiceName--------" << sendServiceName << endl;
	cout << "message--------" << message << endl;

	return 0;
}
