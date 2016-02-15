#include <iostream>
#include <string.h>

#include "MQPool/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

using namespace std;

#define THEFILE "key3"

int main() {

  ShmAllocator shmA(true);

  cout << "11111111111111111" << endl;

  shmA.Attach();

  cout << "22222222222222222" << endl;

  shmA.InitPHead();

  cout << "333333333333333333" << endl;

  uint64_t offset;
  void* pt = shmA.Allocate(1, offset);

  cout << "4444444444444444444" << endl;

  char* ptr = (char*)pt;
  *ptr = 'a';
  cout << "the offset is " << offset << endl;

  shmA.Dump();

  /*char* pp = (char*)shmA.GetMemByOffset(4194376);
  cout << *pp << endl;

  char* tt = (char*)shmA.GetShmAddr();
  char* qq = tt + 4194376;
  cout << *qq << endl;
  cout << "the offset is " << offset << endl;
*/
  /*char* mm = (char*)shmA.GetShmAddr();
   char* dd = mm + 4194303;
   *dd = 'r';*/
   /*cout << *dd << endl;*/


  /*shmA.Dump();*/










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
