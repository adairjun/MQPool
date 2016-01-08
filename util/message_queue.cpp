#include "MQPool/message_queue.h"
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

int MessageQueue::SendMsg(struct rapidMsg* message) {
  return msgsnd(msgid_, message, sizeof(struct rapidMsg), 0);
}

int MessageQueue::RecvMsg(long type, struct rapidMsg* messagePtr) {
  return msgrcv(msgid_, messagePtr, sizeof(struct rapidMsg), type, 0);
}

void MessageQueue::DeleteMsgQue() {
  msgctl(msgid_, IPC_RMID, NULL);
}
