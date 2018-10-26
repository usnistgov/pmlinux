#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>

/*
Data structure holding
decision information.
*/

typedef struct
{
  unsigned int process_id;
  unsigned int user_id;
  char pathname[1000];
  char system_call[10];
}decision_info;

int db_fun (unsigned int process_id, unsigned int user_id, char *pathname, char *system_call);

int main(void) {

  int running;
  int answer;
  decision_info request_info;

  running = 1;

  //Set up where we're writing decision information in module
  syscall(SYS_read, -100, &request_info, 1);

  //Start policy machine within the kernel
  syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 100);

  //Continuous loop for the running policy machine
  while (running > 0) {
    running++;
    //Call function to get answer
    if(strcmp(request_info.system_call, "registration") == 0) {
      printf("reg call\n");
      syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 100);
    }
  
      answer = db_fun (request_info.process_id, request_info.user_id, request_info.pathname, request_info.system_call);

    //If answer returns yes
      if (answer) {
	printf("yes\n");
	syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 1);
      }

    //If answer returns no
      else {
	printf("no\n");
	syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 0);
      }

      if (running == 5)
	syscall(SYS_setxattr, "/.../policyupdate", "/.../policyupdate", "/.../policyupdate", 100, 0);
    }
  

  return 0;

}

//Function to determine answer
int db_fun (unsigned int process_id, unsigned int user_id, char *pathname, char *system_call) {
  FILE *fp;
  char *name;
  char str[10];
  char num[5];
  
  sprintf(num, "%d", user_id);

  fp = fopen("db.txt", "r"); 

  //Read for system call
  while (fscanf(fp, "%s", str) != 0 && strstr(str, system_call) == NULL) {
    strcpy(str, "");
  }
  
  strcpy(str, "");

  //After finding system call, look for path name within that system call
  while (fscanf(fp, "%s", str) != 0 && strstr(pathname, str) == NULL) {
    strcpy(str, "");
  }
  
  strcpy(str, "");
  fgets(str, 1000, fp);
  //Grab id line
  fgets(str, 1000, fp);

  //If id is in line, then we have permission
  if (strstr(str, num) != NULL) {
    fclose(fp);
    return 1;
  }

  //Otherwise we don't
  fclose(fp);
  return 0;
}
