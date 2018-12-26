void set_user_nice(struct task_struct *p, long nice)
{
    bool queued, running;
    int old_prio, delta;
    struct rq_flags rf;
    struct rq *rq;
    if (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE) 
        return;
    rq = task_rq_lock(p, &rf);       
    update_rq_clock(rq);           
    if (task_has_dl_policy(p) || task_has_rt_policy(p)) 
	{ 
        p->static_prio = NICE_TO_PRIO(nice);  
        goto out_unlock;            
    }
    queued = task_on_rq_queued(p);
    running = task_current(rq, p);     
    if (queued)                    
        dequeue_task(rq, p, DEQUEUE_SAVE | DEQUEUE_NOCLOCK);  
    if (running)                   
        put_prev_task(rq, p);

    p->static_prio = NICE_TO_PRIO(nice); 
    set_load_weight(p);                  
    
    old_prio = p->prio;           
    p->prio = effective_prio(p);       
    delta = p->prio - old_prio;
    if (queued) {
        //如果要设置的nice的task在queue中
        enqueue_task(rq, p, ENQUEUE_RESTORE | ENQUEUE_NOCLOCK);
        if (delta < 0 || (delta > 0 && task_running(rq, p)))  
            resched_curr(rq); //重新调度rq。
    }
    if (running) //如果要设置nice值的task正在运行，由于我们这里改变了p的优先级，则重新指定task的rq.
        set_curr_task(rq, p);
		out_unlock:
    	task_rq_unlock(rq, p, &rf);
}