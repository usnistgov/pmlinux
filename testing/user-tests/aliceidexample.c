#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
  
  int i;
  int id;

  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/hello-file/hello.txt", O_RDWR);

  if (id > 0) {
    printf("hello\n");
    printf("Opened with success\n");
  }
  
  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/test1.txt", O_RDWR);

  if (id > 0) {
    printf("test1\n");
    printf("Opened with success\n");
  }
  
  char buf[1000] = "hello";
  i = syscall(SYS_read, id, buf, 11);

  if (i > 0) {
    printf("%s", buf);
    printf("Read with success\n");
  }

  i = syscall(SYS_write, id, "test write alice", 17);

  if (i > 0)
    printf("wrote with success\n");
  
  syscall(SYS_close, id);

  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/test-file/test1/test.txt", O_RDWR);

  if (id > 0) {
    printf("test\n");
    printf("Opened with success\n");
  }
  
  i = syscall(SYS_read, id, buf, 17);

  if (i > 0) {
    printf("%s", buf);
    printf("Read with success\n");
  }
  
  i = syscall(SYS_write, id, "write from alice", 17);

  if (i > 0)
    printf("Wrote with success\n");
  
  syscall(SYS_close, id);

  return 0;
}
  
  
