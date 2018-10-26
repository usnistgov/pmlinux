#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
  
  int i;
  int id;
  
  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/hello-file/hello.txt", O_RDWR);

  if (id > 0)
    printf("Opened with success\n");
  
  char buf[20] = "hello";
  i = syscall(SYS_read, id, buf, 10);

  if (i > 0) {
    printf("%s\n", buf);
    printf("Read with success\n");
  }
  
  i = syscall(SYS_write, id, "test write", 11);
  if (i > 0)
    printf("Wrote with success\n");
  
  printf("%s\n", buf);
  syscall(SYS_close, id);

  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/test1.txt", 0);

  if (id > 0)
    printf("Opened with success\n");
  
  i = syscall(SYS_read, id, buf, 11);

  if (i > 0) {
    printf("%s\n", buf);
    printf("Read with success\n");
  }
  
  syscall(SYS_close, id);
  
  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/test-file/test1/test.txt", 0);

  if (id > 0)
    printf("Opened with success\n");
  
  i = syscall(SYS_read, id, buf, 11);

  if (i > 0) {
    printf("%s\n", buf);
    printf("Read with success\n");
  }

  syscall(SYS_close, id);
  
  return 0;
}
  
  
