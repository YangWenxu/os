//一个基本的内核模块
#include<linux/init.h>//包含模块初始化和清理函数的定义
#include<linux/module.h>//包含大量加载模块所需的函数和符号定义
#include<linux/kernel.h>//包含printk(日志级别"消息文本");定义
#include <linux/sched.h>
//模块动态载入内核
static int hello_init(void)//初始化：模块注册和申请资源
{
        struct task_struct *p;
        printk(KERN_ALERT"名 称 \t 进程号 \t 状态 \t 优先级\t");
        for_each_process(p)
        {
                if(p->mm == NULL)//内核线程的mm成员为空,筛选内核级线程
                  printk(KERN_ALERT"%s\t%d\t%d\t%d\n",p->comm,p->pid, p->state,p->normal_prio);
        }
        return 0;//返回0表示初始化成功
}
//模块动态卸载内核
static void hello_exit(void)//退出：注销和释放资源
{
        printk(KERN_ALERT "list over !\n");
}
module_init(hello_init);//加载函数
module_exit(hello_exit);//卸载函数
MODULE_LICENSE("GPL");  //模块许可申明：宏声明此模块的许可证


