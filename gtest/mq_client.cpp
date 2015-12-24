#include <iostream>


#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/msg.h>  
#include <sys/stat.h>  
#include "MQPool/message_factory.h"

using namespace std;
#define MSG_FILE "key"  

#define PERM S_IRUSR|S_IWUSR  

int main(int argc,char **argv)  
{  
	struct myMsg msg = MessageFactory::Instance().CreateMyMsg(1, MessageFactory::REQUEST, "SENDSERVICE", "MESSAGEBODY");
	key_t key;  
	int msgid;  
	/*if(argc!=2)
	{  
		fprintf(stderr,"Usage：%s string\n\a",argv[0]);  
		exit(1);  
	}  */

	cout << msg.buffer[0] << endl;
	cout << msg.buffer[1] << endl;
	cout << msg.buffer[2] << endl;
	cout << msg.buffer[3] << endl;
	cout << msg.buffer[4] << endl;

	if((key=ftok(MSG_FILE,'a'))==-1)  
	{  
		fprintf(stderr,"Creat Key Error：%s\a\n",strerror(errno));  
		exit(1);  
	}  
	if((msgid=msgget(key,PERM))==-1)  
	{  
		fprintf(stderr,"Creat Message Error：%s\a\n",strerror(errno));  
		exit(1);  
	}  
	/*msg.messageId =1;
	strncpy(msg.buffer, argv[1], _MYMSG_BUFFER_);*/
	msgsnd(msgid,&msg,sizeof(struct myMsg),0);
	memset(&msg,'\0',sizeof(struct myMsg));
	msgrcv(msgid,&msg,sizeof(struct myMsg),2,0);
	printf("Client receive：%s\n",msg.buffer + 9);
	exit(0);  
}
