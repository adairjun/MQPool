#include <iostream>
#include <sys/shm.h>
#include <sys/stat.h>

using namespace std;

#define THEFILE "key"

int main() {
  key_t key = ftok(THEFILE, 'a');
 
  int shmid = shmget(key, 10, S_IRUSR|S_IWUSR|IPC_CREAT);
  int* ptr = (int*)shmat(shmid, NULL, 0);
  *ptr = 4567;
  return 0;
}
