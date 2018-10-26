#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
  
  int i;
  int id;
  
  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/hello-file/hello.txt", O_RDWR);
    
  char buf[20] = "hello";
 
  if (id > 0)
    printf("Opened with success\n");
  
  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/test1.txt", O_RDWR);

  if (id > 0)
    printf("Opened with success\n");
  
  id = syscall(SYS_open, "/home/kfulton/access-control/pm-files/test-file/test1/test.txt", O_RDWR);

  if (id > 0)
    printf("Opened with success\n");
    
  return 0;
}
  
  
