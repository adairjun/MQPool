#include "MQPool/shm_allocator.h"
#include <sys/types.h>
#include <sys/ipc.h>

ShmAllocator::ShmAllocator() {

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
