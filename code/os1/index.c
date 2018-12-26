SYSCALL_DEFINE5(mysetnice, pid_t, pid, int, flag, int, nicevalue, void __user *, prio, void __user *, nice)
{
	struct pid * kpid = find_get_pid(pid);
	struct tast_struct * task = pid_task(kpid,PIDTYPE_PID);
	if(flag==1){
		set_user_nice(task,nicevalue);
		int ni = task_nice(task);
		int pr = task_prio(task);
		copy_to_user(nice,&ni,sizeof(ni));
		copy_to_user(prio,&pr,sizeof(pr));
		printk("修改前的nice值：%d\t修改后的nice值：%d\n", ni, nicevalue);
	} else if( flag ==0 ) {
		int ni = task_nice(task);
		int pr = task_prio(task);
		copy_to_user(nice,&ni,sizeof(ni));
		copy_to_user(prio,&pr,sizeof(pr));
		printk("该进程的nice值是：%d\n", ni);
		printk("该进程的prio值是：%d\n", pr);
		return 0;
	}
	return EFAULT;
}