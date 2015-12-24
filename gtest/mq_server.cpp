#include <iostream>
#include <string.h>  

#include "MQPool/message_queue.h"

using namespace std;


int main()  
{  
	MessageQueue myQueue("key");
	struct myMsg msg;

	while(1)  
	{  
		myQueue.RecvMsg(1, &msg);

		long messageId;
		MessageFactory::MessageType messageType_;
		string sendServiceName;
		string message;
		MessageFactory::Instance().ParseMyMsg(msg, messageId, messageType_, sendServiceName, message);


        cout << "sendServiceName--------" << sendServiceName << endl;
        cout << "message--------" << message << endl;

        msg = MessageFactory::Instance().CreateMyMsg(2, MessageFactory::REQUEST, "mq_server", "THIS IS RESPONSE");
		myQueue.SendMsg(&msg);
	}  
	exit(0);  
}
