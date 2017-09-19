#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ttl.h"
#include "server.h"


void *start_ttl(void *thread_id)
{
	printf("Starting TTL loop\n");
    ttl_start();
	ttl_loop();

    pthread_exit(NULL);
}

void *start_http(void *thread_id)
{
	printf("Starting HTTP Server\n");
	server_start();

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	usleep(100000000);
	pthread_t ttl_thread, server_thread;
 	pthread_attr_t attr;
 	void *status;

 	/* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&ttl_thread, &attr, start_ttl, NULL);
    pthread_create(&server_thread, &attr, start_http, NULL);
    
    pthread_attr_destroy(&attr);

    pthread_join(ttl_thread, &status);
    pthread_join(server_thread, &status);

 	pthread_exit(NULL);
}