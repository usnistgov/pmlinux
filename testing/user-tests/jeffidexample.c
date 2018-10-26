#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
  
  int i;
  int id;
  
  id = syscall(SYS_open, "/home/kfulton/pm-linux/testing/pm-test-files/hello-file/hello.txt", O_RDWR);
    
  char buf[20] = "hello";
 
  if (id > 0)
    printf("Opened hello.txt with success\n");
  
  id = syscall(SYS_open, "/home/kfulton/pm-linux/testing/pm-test-files/test1.txt", O_RDWR);

  if (id > 0)
    printf("Opened test1.txt with success\n");
  
  id = syscall(SYS_open, "/home/kfulton/pm-linux/testing/pm-test-files/test-file/test1/test.txt", O_RDWR);

  if (id > 0)
    printf("Opened test.txt with success\n");
    
  return 0;
}
  
  
