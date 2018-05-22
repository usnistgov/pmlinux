#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <asm/paravirt.h>
#include <linux/sched.h>
#include <asm/ptrace.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <asm/unistd.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fcntl.h>
#include <linux/kallsyms.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/workqueue.h>
#include <linux/semaphore.h>
#include <linux/fdtable.h>
#include <linux/fs_struct.h>
#include <linux/flex_array.h>
#include <linux/cred.h>
#include <linux/pid.h>
#include <linux/dcache.h>
#include <linux/path.h>
#include <linux/rcupdate.h>
#include <linux/slab.h>

unsigned long **sys_call_table;
unsigned long original_cr0;

static struct task_struct *sleeping_policy_machine;
static struct task_struct *sleeping_policy_process;

wait_queue_head_t wait_queue;
struct semaphore sem;

int process_waiting = 0;
int answer_waiting = 0;
int machine_asleep = 0;
int policy_machine_running = 0;
long success = 0;
pid_t proc_id;

pid_t pid;
uid_t user_id;
pid_t policy_process;

char syscallname[10];

pid_t procs[100];
int num_procs = 0;
char file_pathname[1000];

void *buffer_pointer = NULL;

//Checks to see if process is policy process
int inProcs(pid_t process_id)
{
  int i;
  for (i = 0; i < 100; i++) {
    if (procs[i] == process_id)
      return 1;
  }

  return 0;
}

asmlinkage long (*getuid)(void);

asmlinkage long (*ref_sys_execve)(const char __user *filename, const char __user *const __user *argv, const char __user *const __user*envp);

asmlinkage long new_sys_execve(const char __user *filename, const char __user *const __user *argv, const char __user *const __user *envp)
{
  long ret;

  if (down_interruptible(&sem))
    return 0;
  
  if (inProcs(task_pid_nr(current -> parent))) {
    policy_process = task_pid_nr(current);
    procs[num_procs] = policy_process;
    num_procs++;
  }

  up(&sem);
ret = do_execve(getname(filename), argv, envp);
return ret;

}

//Get file pathname
void get_path (int fd, const char *pathname, const char *syscall)
{
     
  if (strcmp(syscall, "open") == 0) {
    /*if (strstr(pathname, "/home/") != NULL) {
      strcpy(file_pathname, pathname);
    }
    
    else {
      char *current_dir;
      struct path path;
      char buf[100];
    
      path = current -> fs -> pwd;
      path_get(&path);
      current_dir = d_path(&path, buf, 100*sizeof(char));
      strcpy(file_pathname, current_dir);
      
      }*/
    struct filename *path;
    path = getname(pathname);
    strcpy(file_pathname, path-> name);
    //printk("open %s\n", file_pathname);
  }

  else {
    char buf[1000];
    char *cwd;
    struct file *file;
    struct path *path;

    spin_lock(&current->files->file_lock);
    file = fcheck_files(current->files, fd);

    path = &file->f_path;
    path_get(path);
    spin_unlock(&current->files->file_lock);
    cwd = d_path(path, buf, 1000*sizeof(char));
    path_put(path);
    strcpy(file_pathname, cwd);
    //printk("r/w %s\n", file_pathname);
  }
  
}

//PM from kernel perspective
int pm_blocking (const char *name, int flags, const char *pathname, int fd, const char *syscall)
{
  pid_t *proc;
  uid_t *user;
  char *path;
  char *systemcallname;
  
  if (strcmp(name, "policymachinecall") == 0 && flags == 0) {
    if(down_interruptible(&sem))
      return 0;    
    answer_waiting = 0;
    success = 0;
    wake_up_process(sleeping_policy_process);
    up(&sem);
  }
  
  else if (strcmp(name, "policymachinecall") == 0 && flags == 1) {
    if(down_interruptible(&sem))
      return 0;
    answer_waiting = 0;
    success = 1;
    wake_up_process(sleeping_policy_process);
    up(&sem);
  }

  if (down_interruptible(&sem))
    return 0;

  if (strcmp(name, "policymachinecall") == 0 && process_waiting) {
    process_waiting--;
    sleeping_policy_machine = current;
    wake_up(&wait_queue);
    machine_asleep = 1;
    up(&sem);
    set_current_state(TASK_INTERRUPTIBLE);
    schedule();
    if (down_interruptible(&sem))
      return 0;
    proc = buffer_pointer;
    *proc = pid;
    user = buffer_pointer + sizeof(uid_t);
    *user = user_id;
    path = buffer_pointer + sizeof(uid_t) + sizeof(uid_t);
    printk(file_pathname);
    printk("\n");
    strcpy(path, file_pathname);
    systemcallname = path + 1000;
    printk(syscallname);
    printk("\n");
    strcpy(systemcallname, syscallname);
    up(&sem);
  }

  else if (strcmp(name, "policymachinecall") == 0 && !process_waiting) {
    sleeping_policy_machine = current;
    machine_asleep = 1;
    up(&sem);
    set_current_state(TASK_INTERRUPTIBLE);
    schedule();
    if (down_interruptible(&sem))
      return 0;
    proc = buffer_pointer;
    *proc = pid;
    user = buffer_pointer + sizeof(uid_t);
    *user = user_id;
    path = buffer_pointer + sizeof(uid_t) + sizeof(uid_t);
    printk(file_pathname);
    printk("\n");
    strcpy(path, file_pathname);
    systemcallname = path + 1000;
    printk(syscallname);
    printk("\n");
    strcpy(systemcallname, syscallname);
    up(&sem);
  }
    
  if (strcmp(name, "processcall") == 0 && answer_waiting) {
    process_waiting++;
    while(answer_waiting) {
      DEFINE_WAIT(wait);
      up(&sem);
      prepare_to_wait_exclusive(&wait_queue, &wait, TASK_INTERRUPTIBLE);
      if (answer_waiting)
	schedule();
      finish_wait(&wait_queue, &wait);
      if (down_interruptible(&sem))
	return 0;
      while(!machine_asleep);
    }
  }

  if (strcmp(name, "processcall") == 0 && !answer_waiting) {
    answer_waiting = 1;
    sleeping_policy_process = current;

    user_id = getuid();
    pid = task_pid_nr(current);
    get_path(fd, pathname, syscall);
    strcpy(syscallname, syscall);
    
    if (machine_asleep) {
      machine_asleep = 0;
      wake_up_process(sleeping_policy_machine);
    }
    
    up(&sem);
    set_current_state(TASK_INTERRUPTIBLE);
    schedule();
    return success;
  }

  return 1;
}

asmlinkage long (*ref_sys_setxattr)(const char *path, const char *name, const void *value, size_t size, int flags);

asmlinkage long new_sys_setxattr(const char *path, const char *name, void *value, size_t size, int flags)
{
  
  if (strcmp(name, "/.../policymachinecall") == 0) {
    if (!policy_machine_running && flags != 1 && flags != 0)
      policy_machine_running = 1;
    pm_blocking("policymachinecall", flags, "nonsense", -1, "setxattr");
    return -1;
  }

  if (strcmp(name, "/.../userprocessinit") == 0) {
    if (down_interruptible(&sem))
      return 0;
    policy_process = task_pid_nr(current);
    procs[num_procs] = policy_process;
    num_procs++;
    up(&sem);
    return -1;
  }  

  return (ref_sys_setxattr(path, name, value, size, flags));
}

asmlinkage long (*ref_sys_open)(const char *pathname, int flags);

 asmlinkage long new_sys_open(const char *pathname, int flags)
{
  int ans;

  if (down_interruptible(&sem))
    return 0;
  if (inProcs(task_pid_nr(current)))
  {
    get_path(-1, pathname, "open");
   
    if(strstr(file_pathname, "pm-files") != NULL || strstr(file_pathname, "bobtest.txt") != NULL)
    {
      if (policy_machine_running == 0) {
	up(&sem);
	return -1;
      }
     
      up(&sem);
      ans = pm_blocking("processcall", -1, pathname, -1, "open");
      if (ans) 
	return ref_sys_open(pathname, flags);    
      else
	return -1;
    }

    /*else if (strstr(file_pathname, "/home") == NULL) {
      up(&sem);
      return ref_sys_open(pathname, flags);
    }

    else {
      up(&sem);
      return -1;
      }*/
    up(&sem);
    return ref_sys_open(pathname, flags);
  }
  
  up(&sem);
  return ref_sys_open(pathname, flags);
}

asmlinkage long (*ref_sys_read)(int fd, void *buf, size_t count);

asmlinkage long new_sys_read(int fd, void *buf, size_t count)
{
  int ans;
  
  if (down_interruptible(&sem))
    return 0;

  if (fd == -100) {
    buffer_pointer = buf;
    up(&sem);
    return -1;
  }
  
  if (fd > 0) 
  get_path(fd, "blank", "read");
  if(file_pathname != NULL) {
    if ((strstr(file_pathname, "pm-files") != NULL || strstr(file_pathname, "bobtest.txt") != NULL) && inProcs(task_pid_nr(current))) {

    if (policy_machine_running == 0) {
      up(&sem);
      return -1;
    }
    
    up(&sem);
    ans = pm_blocking("processcall", -1, "blank", fd, "read");
    if (ans) 
      return ref_sys_read(fd, buf, count);
    else
      return -1;
  }
  }

  up(&sem);
  return ref_sys_read(fd, buf, count);
}

asmlinkage long (*ref_sys_write)(int fd, void *buf, size_t count);

asmlinkage long new_sys_write(int fd, void *buf, size_t count)
{
  int ans;

  if (down_interruptible(&sem))
    return 0;

  if (inProcs(task_pid_nr(current))) {
    if (fd > 0)
      get_path(fd, "blank", "read");
    if(file_pathname != NULL) {
      //printk("pathname: %s\n", file_pathname);
      if (strstr(file_pathname, "pm-files") != NULL || strstr(file_pathname, "bobtest.txt") != NULL) {

	if (policy_machine_running == 0) {
	  up(&sem);
	  return -1;
	}

	up(&sem);
	ans = pm_blocking("processcall", -1, "blank", fd, "write");
	if (ans)
	  return ref_sys_write(fd, buf, count);
	else
	  return -1;
      }
    

      else if (strstr(file_pathname, "/home") == NULL) {
	up(&sem);
	return ref_sys_write(fd, buf, count);
      }
    
      else {
	up(&sem);
	return -1;
      }
    }
  }

  up(&sem);
  return ref_sys_write(fd, buf, count);
  
}

static unsigned long **acquire_sys_call_table(void)
{
  unsigned long int offset = PAGE_OFFSET;
  unsigned long **sct;

  while (offset < ULLONG_MAX) {
    sct = (unsigned long **)offset;

    if (sct[__NR_close] == (unsigned long *) sys_close)
      return sct;

    offset += sizeof(void *);
}

  return NULL;
}

static int __init interceptor_start (void)
{

  printk("start\n");
  if(!(sys_call_table = acquire_sys_call_table()))
    return -1;
  
  init_waitqueue_head(&wait_queue);
  sema_init(&sem, 1);
  
  original_cr0 = read_cr0();

  write_cr0(original_cr0 & ~0x00010000);

  getuid = (void *)sys_call_table[__NR_getuid];
  
  ref_sys_execve = (void *)sys_call_table[__NR_execve];
  sys_call_table[__NR_execve] = (unsigned long *)new_sys_execve;
  
  ref_sys_setxattr = (void *)sys_call_table[__NR_setxattr];
  sys_call_table[__NR_setxattr] = (unsigned long *)new_sys_setxattr;

  ref_sys_open = (void *)sys_call_table[__NR_open];
  sys_call_table[__NR_open] = (unsigned long *)new_sys_open;
  
  ref_sys_read = (void *)sys_call_table[__NR_read];
  sys_call_table[__NR_read] = (unsigned long *)new_sys_read;

  ref_sys_write = (void *)sys_call_table[__NR_write];
  sys_call_table[__NR_write] = (unsigned long *)new_sys_write;
  
  write_cr0(original_cr0);

  return 0;
}

static void __exit interceptor_end(void)
{

  if(!sys_call_table) {
    return;
  }
  
  write_cr0(original_cr0 & ~0x00010000);

  sys_call_table[__NR_execve] = (unsigned long *)ref_sys_execve;
  sys_call_table[__NR_setxattr] = (unsigned long *)ref_sys_setxattr;
  sys_call_table[__NR_open] = (unsigned long *)ref_sys_open;
  sys_call_table[__NR_read] = (unsigned long *)ref_sys_read;
  sys_call_table[__NR_write] = (unsigned long *)ref_sys_write;
  write_cr0(original_cr0);

  msleep(2000);
}

module_init(interceptor_start);
module_exit(interceptor_end);
