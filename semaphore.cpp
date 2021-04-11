//  g++ semaphore.cpp -o semaphore -pthread
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem = {0};
    
void *func1(void *arg)
{ 
    while(1)
    {
        printf("thread func1 wait sem......\n");
        sem_wait(&sem);
        printf("thread func1 running.\n");
        printf("-----------------------------\n");
        usleep(1000);
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
    sem_init(&sem, 0, 0);
    pthread_t thread_id[2];
    
    pthread_create(&thread_id[0], NULL, func1, NULL);
    printf("main thread running.\n");
    sleep(2);
    pthread_create(&thread_id[1], NULL, func2, NULL);
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    //sem_destroy(&sem);
    
    return 0;
}
