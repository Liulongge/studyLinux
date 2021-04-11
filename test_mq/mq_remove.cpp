#include<stdio.h>
#include<sys/stat.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<errno.h>
#include <mqueue.h>
#include <string.h>


int main(int argc,char **argv)
{ 
    if(argc != 2)
    {
        fprintf(stderr,"usage mqunlink \n");
        return -1;
        }

        int ret = mq_unlink(argv[1]);
        if(ret != 0)
        {
            fprintf(stderr,"mq_unlink failed %d:%s",
                            errno,strerror(errno));
            return -2;
        }

        return 0;
}