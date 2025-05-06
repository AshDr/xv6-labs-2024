#include "kernel/types.h"
#include "user.h"
int main(int argc, char* argv[]) {
  int pipefd[2];
  int ok = pipe(pipefd);
  if(ok!=0) {
    fprintf(2, "pipe create error\n");
    exit(1);
  }
  char buf[1];
  if(fork()==0) {// in child
    read(pipefd[0], buf, 1);
    int pid=getpid();
    fprintf(1, "%d: received ping\n", pid);
    write(pipefd[1], buf, 1);
    close(pipefd[0]);
    close(pipefd[1]);
    exit(0);
  }else {
    write(pipefd[1], "A", 1);
    close(pipefd[1]);
    wait(0);
    int pid=getpid();
    read(pipefd[0], buf, 1);
    fprintf(1, "%d: received pong\n", pid);
    close(pipefd[0]);
    exit(0);
  }
}