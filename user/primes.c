// #include "kernel/types.h"
// #include "user.h"
// __attribute__((noreturn)) void prime(int lpipe[2]) {
//   close(lpipe[1]);
//   int x;
//   if(read(lpipe[0], &x, sizeof(x))!=sizeof(int)) {
//     exit(0);
//   }
//   printf("prime %d\n", x);
//   int rpipe[2];
//   int ok=pipe(rpipe);
//   if(ok==-1) {
//     fprintf(2, "pipe create error\n");
//     exit(1);
//   }
//   int val;
//   while(read(lpipe[0],&val, sizeof(int))==sizeof(int)) {
//     if(val%x!=0) {
//       write(rpipe[1], &val, sizeof(int));
//     }
//   }
//   close(lpipe[0]);
//   close(rpipe[1]);
//   if(fork()==0) {
//     prime(rpipe);
//   }
//   close(rpipe[0]);
//   wait(0);
//   exit(0);
// }
// int main(int argc, char* argv[]) {
//   int pipefd[2];
//   pipe(pipefd);
//   for(int i=2; i<=130; i++) {
//     write(pipefd[1], &i, sizeof(i));
//   }
//   close(pipefd[1]);
//   if(fork()==0) {
//     prime(pipefd);
//   }
//   close(pipefd[0]);
//   wait(0);
//   exit(0);
// }

#include "kernel/types.h"
#include "user/user.h"

#define MAX 280
#define FIRST_PRIME 2

int generate_natural();                 // -> out_fd
int prime_filter(int in_fd, int prime); // -> out_fd

int main(int argc, char *argv[]) {
  int prime;

  int in = generate_natural();
  while (read(in, &prime, sizeof(int))) {
    // printf(“prime %d: in_fd: %d\n”, prime, in); // debug
    printf("prime %d\n", prime);
    in = prime_filter(in, prime);
  }

  exit(0);
}

// 生成自然数: 2, 3, 4, ..< MAX
int generate_natural() {
  int out_pipe[2];

  pipe(out_pipe);

  if (!fork()) {
    for (int i = FIRST_PRIME; i < MAX; i++) {
      write(out_pipe[1], &i, sizeof(int));
    }
    close(out_pipe[1]);

    exit(0);
  }

  close(out_pipe[1]);

  return out_pipe[0];
}

// 素数筛
int prime_filter(int in_fd, int prime) {
  int num;
  int out_pipe[2];

  pipe(out_pipe);

  if (!fork()) {
    while (read(in_fd, &num, sizeof(int))) {
      if (num % prime) {
        write(out_pipe[1], &num, sizeof(int));
      }
    }
    close(in_fd);
    close(out_pipe[1]);

    exit(0);
  }

  close(in_fd);
  close(out_pipe[1]);

  return out_pipe[0];
}