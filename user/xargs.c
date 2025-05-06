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
int main(int argc, char *argv[]) {
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
  int i = argc;
  int n;
  while ((n=read(0, p, 1)) > 0) {
    printf("test? %c\n",*p);
    if(*p ==' '||*p=='\n') {
      fprintf(1,"test??\n");
      int f=0;
      if(*p=='\n') {
        fprintf(1,"test2\n");
        f=1;
      }
      *p='\0';
      args[i++] = q;
      q = p + 1;
      if (i >= MAXARG) {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
      }
      if(f) {
        args[i]=0;
        for(int j=0; j<i; j++) {
          fprintf(2, "call args: %s\n", args[j]);
        }
        i=argc;
        p=q=buf;
        run(argv[1], args);
      }else ++p;
    }
    else ++p;
  }
  if(i!=argc) {
    args[i]=0;
    for(int j=0; j<i; j++) {
      fprintf(2, "call args: %s\n", args[j]);
    }
    run(argv[1], args);
  }
  printf("n:%d\n",n);
  while(wait(0)!=-1) ;
  exit(0);
}