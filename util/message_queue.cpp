#include "MQPool/message_queue.h"
#include "MQPool/logobj.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

MessageQueue::MessageQueue()
    : msgFile_("key"){
	key_t key = ftok(msgFile_.c_str(),'a');
	msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
	if (msgid_ == -1) {
	  LOG(INFO) << msgFile_ << " is Exist";
	}
	msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT);
}

MessageQueue::MessageQueue(string msgFile)
    : msgFile_(msgFile) {
	key_t key = ftok(msgFile_.c_str(),'a');
	msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
	if (msgid_ == -1) {
	  LOG(INFO) << msgFile_ << " is Exist";
	}
	msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT);
}

MessageQueue::~MessageQueue() {

}

void MessageQueue::Dump() const {
  printf("\n=====MessageQueue Dump START ========== \n");
  printf("msgid_=%d \n", msgid_);
  printf("msgFile_=%s ", msgFile_.c_str());

  printf("\n=====MessageQueue DUMP END ============\n");
}

int MessageQueue::GetMsgID() const {
  return msgid_;
}

string MessageQueue::GetMsgFile() const {
  return msgFile_;
}

void MessageQueue::SetMsgFile(const string& msgFile) {
  msgFile_ = msgFile;
  key_t key = ftok(msgFile_.c_str(),'a');
  msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
  if (msgid_ == -1) {
  	LOG(INFO) << msgFile_ << " is Exist";
  }
  msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT);
}

void MessageQueue::SendMsg(struct rapidMsg* messagePtr) {
  //这里的第三个参数是char数组的长度，不能是sizeof(struct rapidMsg)
  int result = msgsnd(msgid_, messagePtr, _MYMSG_BUFFER_, 0);
  if (result == -1) {
	  LOG(ERROR) << "SendMsg Error!\n";
  }
}

void MessageQueue::RecvMsg(long type, struct rapidMsg* messagePtr) {
   int result= msgrcv(msgid_, messagePtr, _MYMSG_BUFFER_, type, 0);
   if (result == -1) {
	   LOG(ERROR) << "RecvMsg Error!\n";
   }
}

void MessageQueue::DeleteMsgQue() {
  msgctl(msgid_, IPC_RMID, NULL);
}
