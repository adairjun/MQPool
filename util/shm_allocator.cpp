#include "MQPool/shm_allocator.h"
#include "MQPool/logobj.h"
#include <sys/types.h>
#include <sys/ipc.h>

ShmAllocator::ShmAllocator()
    : shmFile_("key") {
	key_t key = ftok(msgFile_.c_str(),'a');
	shmid_ = shmget(key, MAX_BYTES, S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
	if (shmid_ == -1) {
      LOG(INFO) << shmFile_ << " is Exist";
	}
	shmid_ = shmget(key, MAX_BYTES, S_IRUSR|S_IWUSR|IPC_CREAT);
    shmAddr_ = NULL;
}

ShmAllocator::ShmAllocator(string shmFile)
    : shmFile_(shmFile) {
	key_t key = ftok(msgFile_.c_str(),'a');
	shmid_ = shmget(key, MAX_BYTES, S_IRUSR|S_IWUSR|IPC_CREAT|IPC_EXCL);
	if (shmid_ == -1) {
	  LOG(INFO) << shmFile_ << " is Exist";
	}
	shmid_ = shmget(key, MAX_BYTES, S_IRUSR|S_IWUSR|IPC_CREAT);
	shmAddr_ = NULL;
}

ShmAllocator::~ShmAllocator() {

}

void ShmAllocator::Dump() const {
  printf("\n=====ShmAllocator Dump START ========== \n");
  printf("\n=====ShmAllocator DUMP END =============\n");
}


int ShmAllocator::GetShmID() const {
  return shmid_;
}

string ShmAllocator::GetShmFile() const {
  return shmFile_;
}

void ShmAllocator::SetShmFile(const string& shmFile) {
  shmFile_ = shmFile;
  key_t key = ftok(shmFile_.c_str(),'a');
}

void ShmAllocator::Attach() {
  shmAddr_ = shmat(shmid_, NULL, 0);
}

void ShmAllocator::Detach() {
  if (shmAddr_) {
    shmdt(shmAddr_);
  }
}

void* GetShmAddr() const {
  return shmAddr_;
}
