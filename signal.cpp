#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* 信号处理函数 */
void handlerSIGINT( int signo )
{
	printf("recv SIGINT(%d)\n", signo);
    exit(0);
}

void handlerSIGTERM( int signo )
{
	printf("recv SIGTERM(%d)\n", signo);
    exit(-1);
}

int main( void )
{
	/* 注册信号处理函数 */
	signal( SIGINT, handlerSIGINT);
    signal( 15, handlerSIGTERM);
	while(1)
	{
        sleep(1);
    }
	return 0;
}
