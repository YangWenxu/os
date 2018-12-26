#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include "common.h"

// 由于有名信号量的值是保存在文件中的，所以对于相关进程来说，
// 子进程是继承了父进程的文件描述符，那么子进程所继承的文件描述符所指向的文件是和父进程一样的，
// 当然文件里面保存的有名信号量值就共享了。
sem_t *sem_send;
sem_t *sem_recive;

int main()
{
    // init
    char msg[512];
    int shmid = shmget(key, sizeof(message), 0666 | IPC_CREAT);
    check_error(shmid);

    void* shmp = shmat(shmid, NULL, 0); //NUll让系统确定一个合适的地址位置, 0是可以读写

    sem_send = sem_open(name1, O_CREAT, 0666, 1);
    sem_recive = sem_open(name2, O_CREAT, 0666, 0);
    // 特殊的虚拟文件系统文件

    printf("请输入: ");
    scanf("%s", msg);

    // 发送信息,申请信号量作为临界资源
    sem_wait(sem_send);
    strcpy(((message*)shmp)->text, msg);
    sem_post(sem_recive);

    // 接受信息
    sem_wait(sem_send);
    strcpy(msg, ((message*)shmp)->text);
    printf("sender 接受到: %s\n", msg);

    sem_close(sem_send);
	sem_close(sem_recive);

    // 移除有名信号量
	sem_unlink(name1);
	sem_unlink(name2);

    shmctl(shmid, IPC_RMID, shmp); //shmp 用户空间中的一个缓存，接受或提供状态信息

    printf("sender end\n");
    return 0;
}