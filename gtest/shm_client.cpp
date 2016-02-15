#include <iostream>
#include "MQPool/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

using namespace std;

#define THEFILE "key"

int main(int argc, char** argv) {
  ShmAllocator shmAct(false);
  shmAct.Attach();
  char* ptr = (char*)(shmAct.GetMemByOffset(4194376));
  cout << *ptr << endl;

  shmAct.Dump();
  /*key_t key = ftok(THEFILE, 'a');
 
  int shmid = shmget(key, 0, S_IRUSR|S_IWUSR);
  int* ptr = (int*)shmat(shmid, NULL, 0);
  shmctl(shmid, IPC_RMID, NULL);*/

  /*ManagerMem manager(false);
  manager.Attach();
  manager.Dump();*/

  return 0;
}
