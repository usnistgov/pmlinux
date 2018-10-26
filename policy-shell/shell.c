#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>

int main(void) {
  syscall(SYS_setxattr, "/.../userprocessinit", "/.../userprocessinit", "/.../userprocessinit", 100, 100);
  
  char *args[] = {"/bin/bash", NULL};
  execvp(args[0], args);
  
  return 0;
}
