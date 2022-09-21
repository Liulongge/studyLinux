//  g++ semaphore.cpp -o semaphore -pthread

// 参考链接：
// https://blog.csdn.net/amumu_123/article/details/70313307?spm=1001.2014.3001.5506
// https://blog.csdn.net/weixin_36209467/article/details/101195244?spm=1001.2014.3001.5506

// 1、int sem_init(sem_t *sem, int pshared, unsigned int value)
// sem：指向信号量结构的一个指针；
// pshared：不为０时此信号量在进程间共享，否则只能为当前进程的所有线程共享；
// 但LinuxThreads仅实现线程间共享，因此所有非0值的pshared输入都将使sem_init()返回-1，且置errno为ENOSYS。
// 不过两者的实现并没有很大的区别，下面仅讨论线程间的共享。
// value：给出了信号量的初始值。

// 2、int sem_destroy(sem_t * sem)
// 释放自己占用的一切资源，被注销的信号量sem要求已没有线程在等待该信号量了，否则返回-1，且置errno为EBUSY。
// 除此之外，LinuxThreads的信号量，注销函数不做其他动作。

// 3、int sem_post(sem_t * sem)
// 用来增加信号量的值。当一个线程要离开临界区时调用这个函数，给信号量加1。
// 如果有线程阻塞在这个信号量上时，调用这个函数会使其中的一个线程不在阻塞，
// 它是一个“原子操作”，即同时对同一个信号量做加“1”操作的两个线程是不会冲突的

// 4、int sem_wait(sem_t * sem)
// 是一个原子操作，它的作用是从信号量的值减去一个“1”，
// 但它永远会先等待该信号量为一个非零值才开始做减法。
// 也就是说，如果你对一个值为2的信号量调用sem_wait(),线程将会继续执行，信号量的值将减到1。
// 如果对一个值为0的信号量调用sem_wait()，这个函数就会等待直到有其它线程增加了这个值使它不再是0为止。
// 如果有两个线程都在sem_wait()中等待同一个信号量变成非零值，那么当它被第三个线程增加一个“1”时，
// 等待线程中只有一个能够对信号量做减法并继续执行，另一个还将处于等待状态。

// 5、int sem_trywait(sem_t * sem)
// sem_trywait()为sem_wait()的非阻塞版，如果信号量计数大于0，则信号量立即减1并返回0，
// 否则立即返回-1，errno置为EAGAIN。

// 6、int sem_getvalue(sem_t * sem, int * sval)
// 读取sem中信号量计数，存于*sval中，并返回0。

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>

sem_t sem = {0};
    
void *func1(void *arg)
{ 
    while(1)
    {
        printf("thread func1 wait sem......\n");
        sem_wait(&sem);
        printf("thread func1 running.\n");
        printf("-----------------------------\n");
    }
}

void *func2(void *arg)
{
    while(1)
    {
        printf("thread func2 running.\n");
        sem_post(&sem);
        printf("thread func2 post sem.\n");
        sleep(5);
    }
}

    
int main(void)
{
    int status = 0;
    sem_init(&sem, 0, 0);
    pthread_t thread_id[2];
    
    for(int i = 0; i < 5; i++)
    {
        sem_post(&sem);
        int sem_count = 0;
        status = sem_getvalue(&sem, &sem_count);
        std::cout << "[post] current sem count is: " << sem_count << std::endl;
    }
    
    for(int i = 0; i < 5; i++)
    {
        status = sem_trywait(&sem);
        int sem_count = 0;
        status = sem_getvalue(&sem, &sem_count);
        std::cout << "[try wait] current sem count is: " << sem_count << std::endl;
    }

    pthread_create(&thread_id[0], NULL, func1, NULL);
    printf("main thread running.\n");
    sleep(2);
    pthread_create(&thread_id[1], NULL, func2, NULL);
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    //sem_destroy(&sem);
    
    return 0;
}
