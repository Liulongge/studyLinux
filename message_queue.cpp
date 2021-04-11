#include <iostream>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/msg.h>

#define QUEUE_NAME "/test_queue"
mqd_t mqID;
mq_attr mqAttr;

using namespace std;
char send_message[200] = {0};

void *func1(void *arg)
{
    for(int i = 0; i < 10; i++)
    {
        sleep(1);
        snprintf(send_message, 200, "%s%d","hello:", i);
        printf("send to mq`s msg = %s\n", send_message);
        mq_send(mqID, send_message, strlen(send_message), 10);
    }
}
    
void *func2(void *arg)
{
    char buff[8192];
    unsigned int iprio;

    mq_getattr(mqID, &mqAttr);
    
    while(1)
    {
        mq_receive(mqID, buff, mqAttr.mq_msgsize, &iprio);
        printf("read from mq`s msg = %s\n", buff);
    }
}

int main(int argc, char** argv)
{

    mqID = mq_open(QUEUE_NAME, O_RDWR|O_CREAT|O_EXCL, 0666, NULL);
    if (mqID == -1)
    {
        printf("message queue [/test_queue] exit\n");
        int ret = mq_unlink(QUEUE_NAME);
        mqID = mq_open(QUEUE_NAME, O_RDWR|O_CREAT|O_EXCL, 0666, NULL);
    }
    printf("open message queue succ, mqID = %d\n", mqID);
    
    if (mq_getattr(mqID, &mqAttr) < 0)
    {
        printf("get the message queue attribute error\n");
        return -1;
    }
    
    printf("mq_flags = %d, mq_maxmsg = %d, mq_msgsize = %d, mq_curmsgs = %d\n",
        mqAttr.mq_flags, mqAttr.mq_maxmsg, mqAttr.mq_msgsize, mqAttr.mq_curmsgs);
    

    pthread_t thread_id[2];

    pthread_create(&thread_id[1], NULL, func2, NULL);
    printf("creat receive thread success.\n");
    sleep(1);
    pthread_create(&thread_id[0], NULL, func1, NULL);
    printf("creat send thread success.\n");
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    return 0;
}