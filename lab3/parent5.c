#include <sys/ptrace.h>
#include <signal.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/wait.h>
main()
{

    int i = 0, status = 0, pid, dat = 0;
    long addr = 0x00000000004c72f0;
    struct user_regs_struct uregs;

    if ((pid=fork())==0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl("/home/cmy/文档/System Programming in C/lab3/child5", "child5", 0);
        printf("execl error...\n");
    } else {
        wait(&status);
        while(1) {
        // do just one instruction
            ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
            wait(&status);
            if(WIFEXITED(status)) break;
        // `addr' is address of `i' in child process
        dat = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
        if(dat == 10) {
            ptrace(PTRACE_POKEDATA, pid, addr, 2341);
            ptrace(PTRACE_CONT, pid, 0, 0);
            break;
        }
        }
        wait(&status);
        if(WIFEXITED(status))printf("child over\n");
    }
}
