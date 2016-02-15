#include <iostream>
#include <string.h>

#include "MQPool/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

using namespace std;

#define THEFILE "key"

int main() {

  ShmAllocator shmA(true);
  shmA.Attach();
  shmA.InitPHead();
  uint64_t offset;
  char* ptr = (char*)(shmA.Allocate(4, offset));
  *ptr = 'a';

  cout << offset << endl;
  shmA.Dump();

  /*key_t key = ftok(THEFILE, 'a');
 
  int shmid = shmget(key, 10, S_IRUSR|S_IWUSR|IPC_CREAT);
  int* ptr = (int*)shmat(shmid, NULL, 0);
  *ptr = 4567;*/

  /*ManagerMem manager(true);
  manager.Attach();
  manager.InitPHead();
  manager.AddIdOffsetMapping(1,100);
  manager.AddIdOffsetMapping(2,200);
  manager.AddIdOffsetMapping(3,300);
  manager.AddIdOffsetMapping(4,400);
  manager.AddIdOffsetMapping(5,500);
  manager.AddIdOffsetMapping(6,600);
  manager.AddIdOffsetMapping(7,700);

  manager.Dump();*/
  return 0;
}
