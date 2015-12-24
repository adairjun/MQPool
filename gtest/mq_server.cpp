#include <iostream>


#include <stdio.h>
#include <string.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/stat.h>  
#include <sys/msg.h>  
#include "MQPool/message_factory.h"

using namespace std;

#define MSG_FILE "key" 

#define PERM S_IRUSR|S_IWUSR  

int main()  
{  
	struct myMsg msg;
	key_t key;  
	int msgid;  
	if((key=ftok(MSG_FILE,'a'))==-1)  
	{  
		fprintf(stderr,"Creat Key Error：%s\a\n",strerror(errno));  
		exit(1);  
	}  
	if((msgid=msgget(key,PERM|IPC_EXCL))==-1)
	{  
		fprintf(stderr,"Creat Message Error：%s\a\n",strerror(errno));  
		exit(1);  
	}  
	while(1)  
	{  
		msgrcv(msgid,&msg,sizeof(struct myMsg),1,0);
		printf("Server Receive：%s\n",msg.buffer + 9);
		msg.messageId =2;
		msgsnd(msgid,&msg,sizeof(struct myMsg),0);
	}  
	exit(0);  
}
