#include <iostream>
#include "MQPool/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

using namespace std;

#define THEFILE "key3"

int main(int argc, char** argv) {
  ShmAllocator shmAct(false);
  shmAct.Attach();
  void* pt = shmAct.GetMemByOffset(4194303);   // 4 * 1024 * 1024  == 4194304
  char* ptr = (char*)(pt);

  cout << *ptr << endl;



  /*shmAct.Dump();*/







  /*key_t key = ftok(THEFILE, 'a');
 
  int shmid = shmget(key, 0, S_IRUSR|S_IWUSR);
  int* ptr = (int*)shmat(shmid, NULL, 0);
  shmctl(shmid, IPC_RMID, NULL);*/

  /*ManagerMem manager(false);
  manager.Attach();
  manager.Dump();*/

  return 0;
}
