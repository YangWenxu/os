#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define size 40 //写入字节数
int fd[2]; //fd[0]:读管道，fd[1]:写管道

//成功创建子进程，进行写操作
void WritePipe(int id)
{
        printf("Father's child %d,start write!\n",id+1);
        close(fd[0]); //子进程进行写入前先关闭读取管道
        char str[25]; //输入整数
        sprintf(str, "%d", id+1); // 使用sprintf将整数转换成字符串  
	char mesg[100] = "this message from Father's child ";
        strcat(mesg,str); //连接
        int rs = (int)write(fd[1], mesg, size); //write函数写入失败时返回-1，成功时返回写入的字节数
        if(  rs != -1 )
	{
            printf("子进程%d写入完毕,写入的字节数为%d\n",id+1,rs);
        }
        else
	{
            printf("子程序%d写入失败\n",id);
        }
        
        exit(0);
}


int main()
{
	pid_t pid[3]; //3个子进程
	char getMesg[40]; //子进程信息
	
	//创建管道失败
	if(pipe(fd)<0) 
	{
		fprintf(stderr, "Father's create pipe error : %s\n",strerror(errno));
		//标准错误流，内核在处理stdout和stderr时优先级不一样，后者的优先级要高一些
		//因此有时候如果程序异常退出时，stderr能得到输出，而stdout就不行。
		//printf(...)实际上相当于fprintf(stdout, ...)
		//strerror(errno) 错误信息
	}

	//创建子进程1失败
	if ((pid[0]=fork())<0) 
	{
		fprintf(stderr, " Father's Fork 1 Error : %s \n",strerror(errno) );
		exit(EXIT_FAILURE); //EXIT_FAILURE为1，表示没有成功地执行一个程序
	}

	//创建子进程1成功
	if (pid[0]==0) 
	{
	   	WritePipe(0); //写操作
	}
	else
	{
		//创建子进程2失败
		if ((pid[1]=fork())<0) 
	        {
			fprintf(stderr, " Father's Fork 2 Error : %s \n",strerror(errno) );
			exit(EXIT_FAILURE); //EXIT_FAILURE为1，表示没有成功地执行一个程序
	  	}

		//创建子进程2成功
	        if (pid[1]==0)
	 	{
	  		WritePipe(1); //写操作
	  	}
	 	else
	 	{
			//创建子进程3失败		 
			if ((pid[2]=fork())<0) 
		     	{
			    	fprintf(stderr, " Xf's Fork 3 Error : %s \n",strerror(errno) );
			     	//EXIT_FAILURE为1，表示没有成功地执行一个程序,
			    	exit(EXIT_FAILURE);
		      	}

			//创建子进程3成功
		     	if (pid[2]==0)
		      	{
		      		WritePipe(2); //写操作
		      	}
		      	else  
		      	{
				//父进程：阻塞自己等待3个子进程完结
		      		wait(0); 
		      		wait(0); 
		      		wait(0); 
				    close(fd[1]); //读取前关闭写入管道
	    			printf("Father's父进程开始读取\n");
		    
			     	for (int i=0; i<3; i++) 
			       	{
				  	if ( read(fd[0], getMesg, size) != -1 ) 
				     		printf("%s\n",getMesg);
				  	else
				     		printf("读取子进程%d的失败\n",i);              
			       	}
				printf("-------------Father's End of Program-----------\n");
				return 0;
	      		}
	  	}
	}
	return 0;
}

