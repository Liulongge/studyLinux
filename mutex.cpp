#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

void* consume(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        printf("consume begin lock\n");  
        count++;
        printf("consume count: %d\n",count); 
        usleep(300);
        printf("consume over lock\n"); 
        pthread_mutex_unlock(&mutex);
        usleep(500);
    }  
    return NULL;
}

void* produce( void * arg )
{
    while(1)
    {
        pthread_mutex_lock(&mutex );
        printf("product begin lock\n");
        count ++;
        printf("product count: %d\n", count);
        usleep(600);
        printf("product over lock\n");
        pthread_mutex_unlock(&mutex );
        usleep(500);
    }    
    return NULL;
}

int main( void )
{
    pthread_t thread1,thread2;
    pthread_create(&thread1, NULL, &produce, NULL );
    pthread_create(&thread2, NULL, &consume, NULL );
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    return 0;
}
