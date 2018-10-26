/* Used the following resources:
   https://stackoverflow.com/questions/13796786/jstring-return-in-jni-program
   http://www.soulmachine.me/blog/2015/07/23/calling-c-code-from-java-using-jni/
   https://stackoverflow.com/questions/43897532/jni-return-string-array-from-c-c11-and-use-on-java-without-for-loop-on-c-s

   To compile this code:
   gcc -shared -fpic -I/usr/lib/jvm/java-8-openjdk-amd64/include/ -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux pm.c -o libpm.so

   To run this code:
   java -cp . -Djava.library.path=. PMAccess

*/

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <jni.h>
#include <math.h>
#include <string.h>

typedef struct
{
  unsigned int process_id;
  unsigned int user_id;
  char pathname[1000];
  char system_call[10];
  //int success;
}decision_info;

decision_info request_info;
//decision_info request_info;
int success = 0;

JNIEXPORT jobjectArray JNICALL Java_pmtest_init
(JNIEnv *env, jobject jobj)
{

  char conv[10];
  jstring process_id;
  jstring user_id;
  jstring pathname;
  jstring system_call;
  int i;
  jobjectArray ret_info;
  jsize len = 4;
  //int success;
  //request_info.success = 0;

  printf("setxattr init\n");
  
  syscall(SYS_read, -100, &request_info, 1);
  printf("struct set up\n");
  while(success!=33){
    success = syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 100);
    sleep(.5);
  }

  printf("getting info\n");
  //sprintf(conv, "%d", request_info.process_id);
  strcpy(conv, "1");
  process_id= (*env)->NewStringUTF(env, &conv);
  sprintf(conv, "%d", request_info.user_id);
  //strcpy(conv, "1001");
  user_id = (*env)->NewStringUTF(env, &conv);
  strcpy(request_info.pathname, "hello.txt");
  pathname = (*env)->NewStringUTF(env, &(request_info.pathname));
  strcpy(request_info.system_call, "open");
  system_call = (*env)->NewStringUTF(env, &(request_info.system_call));
  
  ret_info = (*env)->NewObjectArray(env, len, (*env)->FindClass(env, "java/lang/String"), 0);

  (*env)->SetObjectArrayElement(env, ret_info, 0, process_id);
  (*env)->SetObjectArrayElement(env, ret_info, 1, user_id);
  (*env)->SetObjectArrayElement(env, ret_info, 2, pathname);
  (*env)->SetObjectArrayElement(env, ret_info, 3, system_call);
  
  printf("returning init\n");
  return ret_info;
  
  //Malloc space for each individual entry in array
  //Copy entries in struct to array
  //Return array

}

JNIEXPORT jobjectArray JNICALL Java_pmtest_yes
(JNIEnv *env, jobject jobj)
{

  
  char conv[10];
  jstring process_id;
  jstring user_id;
  jstring pathname;
  jstring system_call;
  int i;
  jobjectArray ret_info;
  jsize len = 4;
  //request_info.success = 0;
  printf("setxattr yes\n");
  
  success = syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 1);

  while(success!=33){
    success = syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 100);
    sleep(.5);
  }
  
  printf("getting info\n");
  sprintf(conv, "%d", request_info.process_id);
  process_id= (*env)->NewStringUTF(env, &conv);
  sprintf(conv, "%d", request_info.user_id);
  user_id = (*env)->NewStringUTF(env, &conv);
  pathname = (*env)->NewStringUTF(env, &(request_info.pathname));
  system_call =(*env)->NewStringUTF(env, &(request_info.system_call));

  ret_info = (*env)->NewObjectArray(env, len, (*env)->FindClass(env, "java/lang/String"), 0);
  
  (*env)->SetObjectArrayElement(env, ret_info, 0, process_id);
  (*env)->SetObjectArrayElement(env, ret_info, 1, user_id);
  (*env)->SetObjectArrayElement(env, ret_info, 2, pathname);
  (*env)->SetObjectArrayElement(env, ret_info, 3, system_call);
  printf("returning yes\n");
  return ret_info;
  
  //copy entries in struct to array
  //return array
}

JNIEXPORT jobjectArray JNICALL Java_pmtest_no
(JNIEnv *env, jobject jobj)
{
  
  char conv[10];
  jstring process_id;
  jstring user_id;
  jstring pathname;
  jstring system_call;
  int i;
  jobjectArray ret_info;
  jsize len = 4;
  //request_info.success = 0;
  printf("setxattr no\n");
 
  success = syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 0);

  while(success!=33){
    success = syscall(SYS_setxattr, "/.../policymachinecall", "/.../policymachinecall", "/.../policymachinecall", 100, 100);
    sleep(.5);
  }
  
  printf("getting info\n");
  sprintf(conv, "%d", request_info.process_id);
  process_id= (*env)->NewStringUTF(env, &conv);
  sprintf(conv, "%d", request_info.user_id);
  user_id = (*env)->NewStringUTF(env, &conv);
  pathname = (*env)->NewStringUTF(env, &(request_info.pathname));
  system_call =(*env)->NewStringUTF(env, &(request_info.system_call));

  ret_info = (*env)->NewObjectArray(env, len, (*env)->FindClass(env, "java/lang/String"), 0);

  (*env)->SetObjectArrayElement(env, ret_info, 0, process_id);
  (*env)->SetObjectArrayElement(env, ret_info, 1, user_id);
  (*env)->SetObjectArrayElement(env, ret_info, 2, pathname);
  (*env)->SetObjectArrayElement(env, ret_info, 3, system_call);
  printf("returning no\n");
  return ret_info;
  
  //copy entries in struct to array
  //return array
}
