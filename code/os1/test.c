#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#define __NR_mysyscall 335

int main()
{
    int pid = 0;
    int flag = 0;
    int nicevalue = 0;
    int prio = 0;
    int nice = 0;
    int result;

    printf("please input: pid, flag, nicevalue\n");
    scanf("%d %d %d", &pid, &flag, &nicevalue);
    result = syscall(__NR_mysyscall, pid, flag, nicevalue, (void *)&prio, (void *)&nice);
    if (result == 0)
    {
        printf("pid:%d, flag:%d, nicevalue:%d, prio:%d, nice:%d\n", pid, flag, nicevalue, prio, nice);
        return 0;
    }
    printf("some wrong, maybe pid is not exist\n");
    return 0;
}
