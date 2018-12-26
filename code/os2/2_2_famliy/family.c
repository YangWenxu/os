#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/sched.h>//任务调度相关函数
#include <linux/moduleparam.h>//带参数的模块声明

static pid_t pid;//参数声明(pid_t型实际上就是int型)
module_param(pid,int,0644);//参数说明：内核态加载模块时传递参数给模块（模块参数的名字pid,模块参数的数据类型int型，模块参数打访问权限644权限）


static int family_init(void)
{
    struct task_struct *p;//PCB数据结构进程描述符具体是task_struct结构体存储的相关信息
    struct list_head *pp;//双向循环链表的结构list_head
    struct task_struct *psibling;//指向结构体的指针

    p = pid_task(find_vpid(pid), PIDTYPE_PID);//pid_task(进程号，进程类型)函数获取任务的任务描述符信息
    if(p->parent==NULL)
    {
         printk("No Parent\n");
    }
    else
    {
         printk("Parent : %d %s\n",p->parent->pid,p->parent->comm);
    }

    list_for_each(pp,&p->parent->children)
    {
         psibling=list_entry(pp,struct task_struct,sibling);
         printk("sibling %d %s \n",psibling->pid,psibling->comm);
    }
    list_for_each(pp,&p->children)
    {
         psibling=list_entry(pp,struct task_struct,sibling);
         printk("children %d %s \n",psibling->pid,psibling->comm);
    }
    return 0;
}

static void family_exit(void)
{
    printk(KERN_ALERT "famliy end!\n");
}

module_init(family_init);
module_exit(family_exit); 
MODULE_LICENSE("GPL"); 

