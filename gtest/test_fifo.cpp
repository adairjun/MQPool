#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;

const int MAXLINE = 1024;
char serbuf[MAXLINE];
char clibuf[MAXLINE];

int server(int readfd, int writefd) {
  read(readfd, serbuf, MAXLINE);
  cout << "this is server:-------------" << serbuf << endl; 
  memset(serbuf, 0, MAXLINE);
  strcpy(serbuf, "nnnnnnnnnnnnnnn");
  write(writefd, serbuf, strlen(serbuf));
  memset(serbuf, 0, strlen(serbuf));
}

int client(int readfd, int writefd) {
  strcpy(clibuf, "mmmmmm");
  write(writefd, clibuf, MAXLINE);
  memset(clibuf, 0, strlen(clibuf));
  read(readfd, clibuf, MAXLINE); 
  cout << "this is receive-----------------" << clibuf << endl;
  memset(clibuf, 0, strlen(clibuf));
}


int main(int argc, char** argv) {
  int readfd, writefd;
  pid_t childfd;
  if (mkfifo("/home/jun/tmp/fifo.1", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
    cerr << "Can't create fifo1" << endl;
  }

  if (mkfifo("/home/jun/tmp/fifo.2", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
    unlink("/home/jun/tmp/fifo.1");
    cerr << "Can't create fifo2" << endl;
  }
 
  if ((childfd == fork()) == 0) {
    readfd = open("/home/jun/tmp/fifo.1", O_RDONLY, 0);
    writefd = open("/home/jun/tmp/fifo.2", O_WRONLY, 0);

    server(readfd, writefd);
    exit(0);
  }
 
  writefd = open("/home/jun/tmp/fifo.1", O_WRONLY, 0);
  readfd = open("/home/jun/tmp/fifo.2", O_RDONLY, 0);
  client(readfd, writefd);

  waitpid(childfd, NULL, 0);
  close(readfd);
  close(writefd);
  unlink("/home/jun/tmp/fifo.1");
  unlink("/home/jun/tmp/fifo.2");
  return 0;
}
