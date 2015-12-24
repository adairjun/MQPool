#include <iostream>
#include <sys/shm.h>
#include <sys/stat.h>

using namespace std;

#define THEFILE "key"

int main() {
  key_t key = ftok(THEFILE, 'a');
 
  int shmid = shmget(key, 0, S_IRUSR|S_IWUSR|IPC_CREAT);
  int* ptr = (int*)shmat(shmid, NULL, 0);
  shmctl(shmid, IPC_RMID, NULL);
  cout << *ptr << endl;
  return 0;
}
