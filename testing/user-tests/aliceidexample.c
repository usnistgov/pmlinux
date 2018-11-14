#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
  
  int i;
  int id;

  id = syscall(SYS_open, "/home/kfulton/pm-linux/testing/pm-test-files/hello-file/hello.txt", O_RDWR);

  if (id > 0) {
    printf("Opened hello.txt with success\n");
    syscall(SYS_close, id);
  }
  
  id = syscall(SYS_open, "/home/kfulton/pm-linux/testing/pm-test-files/test1.txt", O_RDWR);

  if (id > 0) {
    printf("Opened test1.txt with success\n");
  }
  
  char buf[1000] = "hello";
  i = syscall(SYS_read, id, buf, 11);

  if (i > 0) 
    printf("Read test1.txt with success\n");
  

  i = syscall(SYS_write, id, "test write alice", 17);

  if (i > 0)
    printf("wrote test1.txt with success\n");
  
  syscall(SYS_close, id);

  id = syscall(SYS_open, "/home/kfulton/pm-linux/testing/pm-test-files/test-file/test1/test.txt", O_RDWR);

  if (id > 0) 
    printf("Opened test.txt with success\n");
  
  
  i = syscall(SYS_read, id, buf, 17);

  if (i > 0) 
    printf("Read test.txt with success\n");
  
  i = syscall(SYS_write, id, "write from alice", 17);

  if (i > 0)
    printf("Wrote test.txt with success\n");
  
  syscall(SYS_close, id);

  return 0;
}
  
  
