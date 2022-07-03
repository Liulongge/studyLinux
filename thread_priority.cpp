//  g++ thread_priority.cpp -o demo -lpthread
// test on mac, work
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <iostream>

using namespace std;
void show_thread_priority(pthread_attr_t &attr)
{
    struct sched_param param;
    int rs = pthread_attr_getschedparam( &attr, &param );
    assert( rs == 0 );
    cout << "priority = " << param.sched_priority << endl;
}

void show_schedule_strategy(void)
{
    int policy;
    struct sched_param param;
    pthread_getschedparam(pthread_self(),&policy,&param);
    switch(policy)
    {
        case SCHED_OTHER: printf("SCHED_OTHER\n"); break;
        case SCHED_RR: printf("SCHED_RR\n"); break;
        case SCHED_FIFO: printf("SCHED_FIFO\n"); break;
    }
}

void *Thread1(void* pp)
{
    show_schedule_strategy();
    while(1)
    {
        static int run_num = 0;
        run_num ++;
        if(!(run_num % 100))
        printf("Pthread 1, %d\n", run_num);
        // if(run_num == 4000)
        // {
        //      exit(0);
        // }  
    }
    cout << "Pthread 1 exit" << endl;
}

void *Thread2(void* pp)
{
    show_schedule_strategy();
    while(1)
    {
        static int run_num = 0;
        run_num ++;
        if(!(run_num % 100))
        printf("Pthread 2, %d\n", run_num);
        //usleep(500);
    }
    printf("Pthread 2 exit\n");
}

void *Thread3(void* pp)
{
    show_schedule_strategy();
    while(1)
    {
        static int run_num = 0;
        run_num ++;
        if(!(run_num % 100))
        printf("Pthread 3, %d\n", run_num);
        //usleep(500);
    }
    printf("Pthread 3 exit\n");
}

int main()
{
    int uid;
    uid = getuid();
    if(uid==0)
        printf("The current user is root\n");
    else
        printf("The current user is not root\n");

    pthread_t ppid1,ppid2,ppid3;
    
    struct sched_param param1;
    struct sched_param param2;
    struct sched_param param3;

    pthread_attr_t attr1,attr2,attr3;

    pthread_attr_init(&attr1);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);
          
    // param1.sched_priority = 20;
    // pthread_attr_setinheritsched(&attr1,PTHREAD_EXPLICIT_SCHED);
    // pthread_attr_setschedpolicy(&attr1,SCHED_RR);
    // assert( 0 == pthread_attr_setschedparam(&attr1,&param1));

    param2.sched_priority = 50;
    // pthread_attr_setinheritsched(&attr2,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr2,SCHED_RR);
    pthread_attr_setschedparam(&attr2,&param2);
    assert( 0 == pthread_attr_setschedparam(&attr2,&param2));

    param3.sched_priority = 40;
    // pthread_attr_setinheritsched(&attr3,PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr3,SCHED_RR);
    pthread_attr_setschedparam(&attr3,&param3);
    assert(0 == pthread_attr_setschedparam(&attr3,&param3));
    show_thread_priority(attr3);
    // exit(0);

    pthread_create(&ppid1,&attr1,Thread1,NULL);
    pthread_create(&ppid2,&attr2,Thread2,NULL);
    pthread_create(&ppid3,&attr3,Thread3,NULL);

    pthread_join(ppid1,NULL);
    pthread_join(ppid2,NULL);
    pthread_join(ppid3,NULL);    
    
    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);
    pthread_attr_destroy(&attr3);
    return 0;
}
