#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int n = 0;

  if (argc != 2) {
    // output to stdout
    fprintf(2, "usage: sleep n\n");
    exit(1);
  }

  n = atoi(argv[1]);
  sleep(n);
  exit(0);
}