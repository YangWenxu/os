#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <string.h>
#include <pthread.h>

#define KEY 0
#define SIZE 100
#define EXIT "exit"


int msgid;

typedef struct
{
    char text[SIZE];
    long type;
    int sender;
} msgp;

sem_t send_posix, receive_posix, send1_over_posix, send2_over_posix;

void *sender1()
{
    msgp msg;
    while (1)
    {
        sem_wait(&send_posix);
        printf("sender1输入> ");
        scanf("%s", msg.text);
        if (strcmp(msg.text, EXIT) == 0)
        {
            strcpy(msg.text, "end1");
            msg.type = 0;
            msg.sender = 1;
            msgsnd(msgid, &msg, sizeof(msg), 0); //msgflg发送标志，0的意思是消息队列满时，发送进程将会阻塞，
            sem_post(&receive_posix);
            break;
        }
        else
        {
            msg.type = 1;
            msg.sender = 1;
            msgsnd(msgid, &msg, sizeof(msg), 0);
            sem_post(&receive_posix);
        }
    }
    sem_wait(&send1_over_posix);
    msgrcv(msgid, &msg, sizeof(msg), 0, 0);
    //msg是用来存放接受到的消息内容的缓冲区指针
    //msgtyp=0,表示接受到消息队列中的第一个消息
    //msgflg=0,表示没有可以接收的消息时，接收进程将会阻塞
    printf("sender1输出> %s\n", msg.text);
    sem_post(&send_posix);
    return NULL;
}

void *sender2()
{
    msgp msg;
    while (1)
    {
        sem_wait(&send_posix);
        printf("sender2输入> ");
        scanf("%s", msg.text);
        if (strcmp(msg.text, EXIT) == 0)
        {
            strcpy(msg.text, "end2");
            msg.type = 0;
            msg.sender = 2;
            msgsnd(msgid, &msg, sizeof(msg), 0);
            sem_post(&receive_posix);
            break;
        }
        else
        {
            msg.type = 1;
            msg.sender = 2;
            msgsnd(msgid, &msg, sizeof(msg), 0);
            sem_post(&receive_posix);
        }
    }
    sem_wait(&send2_over_posix);
    msgrcv(msgid, &msg, sizeof(msg), 0, 0);
    printf("sender2输出> %s\n", msg.text);
    sem_post(&send_posix);
    return NULL;
}

void *receiver()
{
    msgp msg;
    int flag1 = 0, flag2 = 0;
    while (1)
    {
        sem_wait(&receive_posix);
        msgrcv(msgid, &msg, sizeof(msg), 0, 0);
        printf("receiver输出> sender%d 发来数据: %s\n", msg.sender, msg.text);
        if (msg.type == 0)
        {
            if (msg.sender == 1)
            {
                strcpy(msg.text, "over1");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sem_post(&send1_over_posix);
                flag1 = 1;
            }
            else if (msg.sender == 2)
            {
                strcpy(msg.text, "over2");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sem_post(&send2_over_posix);
                flag2 = 1;
            }

            if (flag1 && flag2) {
                break;
            }
        }
        else
        {
            sem_post(&send_posix);
        }
    }

    return NULL;
}

int main()
{
    sem_init(&send_posix, 0, 1); //pshared不为０时此信号量在进程间共享，否则只能为当前进程的所有线程共享；value给出了信号量的初始值。
    sem_init(&receive_posix, 0, 0);
    sem_init(&send1_over_posix, 0, 0);
    sem_init(&send2_over_posix, 0, 0);
    msgid = msgget(KEY, IPC_CREAT);
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, sender1, NULL); //第一个NULL是创建时的属性，第二个是回调中的参数
    pthread_create(&tid2, NULL, sender2, NULL);
    pthread_create(&tid3, NULL, receiver, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);//主线程等待子线程的终止。也就是在子线程调用了pthread_join()方法后面的代码，只有等到子线程结束了才能执行。

    msgctl(msgid, IPC_RMID, 0);//IPC_RMD,删除标记，并唤醒该消息队列上等待的进程, 
    //buf 用户空间中的一个缓存，接受或提供状态信息 成功返回0
    return 0;
}
