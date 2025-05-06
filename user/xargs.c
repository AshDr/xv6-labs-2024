#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/param.h"
void run(char *prog, char *args[]) {
  if(fork() == 0) {
    exec(prog, args);
    fprintf(2, "xargs: exec %s failed?\n", prog);
    exit(1);
  }
}
int main(int argc, char *argv[]) { // xargs grep hello a.c
  if (argc < 2) {
    fprintf(2, "xargs: usage: xargs [command]\n");
    exit(1);
  }
  char buf[512];
  char *p=buf,*q=buf;
  char *args[MAXARG];
  for(int i=1; i<argc; i++) {
    args[i-1]=argv[i];
  }
  int i = argc-1;
  int n;
  while ((n=read(0, p, 1)) > 0) {
    if(*p ==' '||*p=='\n') {
      int f=0;
      if(*p=='\n') {
        f=1;
      }
      *p='\0';
      args[i++] = q;
      // fprintf(1, "my args:%s\n", args[i-1]);
      q = p + 1;
      if (i >= MAXARG) {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
      }
      if(f) {
        args[i]=0;
        // for(int j=0; j<i; j++) {
        //   fprintf(1, "call args: %s\n", args[j]);
        // }
        i=argc-1;
        p=q=buf;
        run(argv[1], args);
      }else ++p;
    }
    else ++p;
  }
  if(i!=argc-1) {
    args[i]=0;
    // for(int j=0; j<i; j++) {
    //   fprintf(2, "call args: %s\n", args[j]);
    // }
    run(argv[1], args);
  }
  while(wait(0)!=-1) ;
  exit(0);
}