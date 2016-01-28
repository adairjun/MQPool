#include <iostream>
#include <string.h>

#include "MQPool/shm_allocator.h"
#include "rapidmsg/rapidmsg.pb.h"
#include "rapidmsg/test.151000.153000.pb.h"
#include <boost/lexical_cast.hpp>

using namespace std;

#define THEFILE "key"

int main() {

  ShmAllocator shmA(THEFILE);
  /*void* ptr = shmA*/

  /*key_t key = ftok(THEFILE, 'a');
 
  int shmid = shmget(key, 10, S_IRUSR|S_IWUSR|IPC_CREAT);
  int* ptr = (int*)shmat(shmid, NULL, 0);
  *ptr = 4567;*/
  return 0;
}
