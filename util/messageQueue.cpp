#include "MQPool/messageQueue.h"
#include "MQPool/parse_xml.h"
#include "MQPool/parse_json.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>

MessageQueue::MessageQueue()
    : msgFile_("key"){
	key_t key = ftok(msgFile_.c_str(),'a');
	msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
}

MessageQueue::MessageQueue(string msgFile)
    : msgFile_(msgFile) {
	key_t key = ftok(msgFile_.c_str(),'a');
	msgid_ = msgget(key,S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
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
}

int MessageQueue::SendMsg(struct myMsg* message) {
  return msgsnd(msgid_, message, sizeof(myMsg), 0);
}

int MessageQueue::RecvMsg(long type, struct myMsg* messagePtr) {
  return msgrcv(msgid_, messagePtr, sizeof(myMsg), type, 0);

}

void MessageQueue::DeleteMsgQue() {
  msgctl(msgid_, IPC_RMID, NULL);
}
