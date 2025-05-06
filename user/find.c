#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
void find(char *path, char *name) {
  // fprintf(2, "find: %s %s\n", path, name);
  char buf[512],*p;
  int fd = open(path, O_RDONLY);
  if(fd < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  struct stat st;
  struct dirent de;
  if(fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch(st.type) {
    case T_DEVICE:
    case T_FILE:
      if(strcmp(fmtname(path), name) == 0) {
        fprintf(2,"%s T_FILE\n", path);
      }
      break;
    case T_DIR:
      // fprintf(2,"%s T_DIR\n", path);
      if(strlen(path)+1+DIRSIZ+1 > sizeof buf) {
        fprintf(2,"find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      struct stat newst;
      while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0) continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0; // '/0'
        if(stat(buf, &newst) < 0) { // use full path name not de.name
          fprintf(2, "find: cannot stat %s\n", buf);
          continue;
        }
        
        if(newst.type==T_FILE && strcmp(de.name, name) == 0) {
          fprintf(2,"%s\n", buf);
        }else if(newst.type==T_DIR) {
          find(buf, name);
        }
      }
      break;
    default: break;
  }
  close(fd);
}
int main(int argc, char* argv[]) {
  if(argc != 3) {
    fprintf(2, "find: too few arguments\n");
    exit(1);
  }
  char* path = argv[1];
  char* name = argv[2];
  find(path, name);
  exit(0);
}