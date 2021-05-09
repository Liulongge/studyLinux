
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  
#include <sys/prctl.h>

void thread()
{
    //prctl(PR_SET_NAME,"THREAD1");
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(),&policy,&param);

    printf("Scheduling priority: %d\n", param.sched_priority);
    switch(policy)
    {
        case SCHED_OTHER : printf("Scheduling policy: SCHED_OTHER\n"); break;
        case SCHED_FIFO : printf("Scheduling policy: SCHED_FIFO\n"); break;
        case SCHED_RR : printf("Scheduling policy: SCHED_RR\n"); break;
        default : printf("Scheduling policy: unknow\n"); break;
    }

    while(1)
    {
        usleep(100*1000);
        printf("thread\n");
    }  
}

void thread2()
{
    //prctl(PR_SET_NAME,"THREAD2");
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(),&policy,&param);

    printf("Scheduling priority: %d\n", param.sched_priority);
    switch(policy)
    {
        case SCHED_OTHER : printf("Scheduling policy: SCHED_OTHER\n"); break;
        case SCHED_FIFO : printf("Scheduling policy: SCHED_FIFO\n"); break;
        case SCHED_RR : printf("Scheduling policy: SCHED_RR\n"); break;
        default : printf("Scheduling policy: unknow\n"); break;
    }

    while(1)
    {
        usleep(100*1000);
        printf("thread2\n");
    }  
}

int main()
{
    int policy = SCHED_RR;
    //查询当前调度策略支持的最大和最小的优先级值可以用函数：
    int max_ = sched_get_priority_max( policy );
    int min_ = sched_get_priority_min( policy );
    printf("priority max: %d, min: %d\n", max_, min_);

    int i = getuid();
    if(i==0)
        printf("The current user is root\n");
    else
        printf("The current user is not root\n");

    pthread_t pthread, pthread2;
    struct sched_param param, param2, param_thread;
    pthread_attr_t attr, attr2;

    /**** 设置进程调度策略  ****/
    param_thread.sched_priority = 10;
    sched_setscheduler(getpid(), SCHED_RR, &param_thread);    // 设置当前进程调度策略
    printf("thread pid: %d\n",getpid());                      // 获取进程pid
    // 1.如果存在pid所描述的进程，将返回进程ID等于pid的进程的调度策略。
    // 2.如果pid为零，将返回调用进程的调度策略。
    // 3.如果进程pid包含多个进程或轻量进程，此函数只返回进程调度策略和优先级。
    // 目标进程的各个进程或轻量级进程具有其自身的调度策略和优先级，它们可能与当前进程的调度策略和优先级不同。
    policy = sched_getscheduler(getpid());                    //函数返回pid所指定进程的调度策略。
    switch(policy)
    {
        case SCHED_OTHER : printf("Scheduling policy: SCHED_OTHER\n"); break;
        case SCHED_FIFO : printf("Scheduling policy: SCHED_FIFO\n"); break;
        case SCHED_RR : printf("Scheduling policy: SCHED_RR\n"); break;
        default : printf("Scheduling policy: unknow\n"); break;
    }

    // 初始化属性线程属性
    pthread_attr_init(&attr);
    // 设置线程的属性成为完全分离状态，再也不能用pthread_join来等待这条线程
	// PTHREAD_CREATE_JOINABLE
    // PTHREAD_CREATE_DETACHED
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
    // 设置线程是否继承父线程调度策略
    // PTHREAD_EXPLICIT_SCHED：不继承，只有不继承父线程的调度策略才可以设置线程的调度策略
    // PTHREAD_INHERIT_SCHED：继承父进程的调度策略
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    // 设置线程优先级范围
    // int pthread_attr_getscope( const pthread_attr_t * attr, int * scope );
    // int pthread_attr_setscope( pthread_attr_t*, int scope );
    // __scope，表示线程间竞争CPU的范围，也就是说线程优先级的有效范围。
    // POSIX的标准中定义了两个值：PTHREAD_SCOPE_SYSTEM和PTHREAD_SCOPE_PROCESS，
    // 前者表示与系统中所有线程一起竞争CPU时间，后者表示仅与同进程中的线程竞争CPU。
    // 目前LinuxThreads仅实现了PTHREAD_SCOPE_SYSTEM一值。
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);
    // 设置堆栈大小
    pthread_attr_setstacksize(&attr, 16*1024*1024);
    // 设置当前线程调度策略
    pthread_attr_setschedpolicy(&attr, policy);
    // 设置当前线程调度策略及优先级
    param.sched_priority = 10;     
    pthread_setschedparam(pthread_self(), policy, &param);
    // 设置线程名字  
    prctl(PR_SET_NAME,"THREAD1");
    // 创建线程
    pthread_create(&pthread,&attr,(void *)thread,NULL);          



    pthread_attr_init(&attr2);
    param2.sched_priority = 10;
    pthread_setschedparam(pthread_self(), SCHED_RR, &param2);    // 设置当前线程
    prctl(PR_SET_NAME,"THREAD2");
    pthread_attr_setscope (&attr2, PTHREAD_SCOPE_SYSTEM);
    pthread_create(&pthread2,&attr2,(void *)thread2,NULL);

    prctl(PR_SET_NAME,"pthread");
    pthread_attr_destroy(&attr);    // 线程属性销毁
    pthread_attr_destroy(&attr2);
    //线程的属性成为完全分离状态，再也不能用pthread_join来等待这条线程
    //pthread_join (pthread, NULL);  // //等待线程退出

    pthread_join(pthread2,NULL);

    return 0;
}

