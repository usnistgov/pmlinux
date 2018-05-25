# pmlinux
Kernel support for the policy machine at https://github.com/PM-Master.

Kernel module offers support to help enforce policy machine function at the operating system level.

Development is in the very early stages.

The following steps are required to functionally use the module:

1) Module is only compatible with the Linux operating system.

2) The user must have admin privileges on the system in order to insert the module.

3) In order to run the current version of the module, you must add two lines to the operating system code and recompile the kernel.

    The first line you must add is in /fs/exec.c:

        EXPORT_SYMOBL(do_execve)

    The second line you must add is in /fs/namei.c:

        EXPORT_SYMBOL(getname)

Adding these lines will allow these functions to be called and used in the kernel module.

4) Once you add these lines, you have to recompile the operating system code.

For those of you unfamiliar with compiling operating system code, the following link offers instructions and tips:

    https://wiki.ubuntu.com/Kernel/BuildYourOwnKernel

Once your code is recompiled, we are ready to boot the new kernel code.

5) In order to compile the code, the Makefile and kernel-pm.c code must be in the same folder.

6) To compile, run the following command: make.

7) To insert/use the module, run the following command: sudo insmod kernel-pm.ko. The system will prompt you to enter your password. 
