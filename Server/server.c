#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "threadQueue.h"
#include "threadQueue.c"

#define MAX 1024
#define PORT 8082
#define SA struct sockaddr
#define Server_handle 100
#define thread_handling 2

pthread_t sourceOfThread[thread_handling];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t condition_check = PTHREAD_COND_INITIALIZER;


void  *commute(void * client){
	int confd = *((int*)client);
	free(client);

	char readBuffer[MAX], writeBuffer[MAX];
	int n;
	int check_read;
	while(1){
		bzero(readBuffer, MAX);
		read(confd, readBuffer, sizeof(readBuffer));
		printf("From Client: %s\n" ,readBuffer);
		if (strncmp(readBuffer, "HELO Server", 11) == 0)
		{
			/* code */
			write(confd, "200 Hello Client", strlen("200 Hello Client"));
			printf("Sending to client: 200 Hello Client\n");
		}
		else if (strncmp(readBuffer, "QUIT", 4) == 0)
		{
			/* code */
			write(confd, "500 bye", sizeof("500 bye"));
			printf("Sending to client: 500 bye\n");
			break;
		}
		else if (strncmp(readBuffer, "DOWNLOAD", 8) == 0)
		{
			/* code */
			printf("Sending to client: 300 Download File OK\n");
			write(confd, "300 Download File OK", strlen("300 Download File OK"));
			
			bzero(readBuffer, MAX);
			read(confd, readBuffer, sizeof(readBuffer));
			printf("From Client: %s\n" ,readBuffer);

			FILE *file;
			file = fopen(readBuffer, "rb");
			if (file == NULL)
			{
				/* code */
				printf("Sending to client: 305 File Not Exists\n");
				write(confd, "305 File Not Exists", strlen("305 File Not Exists"));
			} else {
				strcpy(readBuffer, "310 File Found");
				printf("Sending to client: %s\n", readBuffer);
				write(confd, readBuffer, strlen(readBuffer));
				
				bzero(readBuffer, MAX);
				check_read = read(confd, readBuffer, sizeof(readBuffer));
				if (check_read <= 0)
				{
					/* code */
					perror("Read error");
					continue;
				}
				readBuffer[check_read] = '\0';

				fseek(file, 0, SEEK_END);
				int size = ftell(file); 
				fseek(file, 0, SEEK_SET);

				printf("Sending to client: File size: %d bytes \n", size);
				snprintf(writeBuffer, sizeof(writeBuffer), "File size: %d bytes", size); 
				write(confd, writeBuffer, strlen(writeBuffer));

				size_t a;
				while(1) {
					check_read = read(confd, readBuffer, sizeof(readBuffer));
					if (check_read <= 0)
					{
						/* code */
						perror("Read error");
						break;
					}
					readBuffer[check_read] = '\0';
					if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
					{
						/* code */
						writeBuffer[a] = '\0';
						write(confd, writeBuffer,a);
					} else {
						strcpy(writeBuffer, "320 End File");
						write(confd, writeBuffer, strlen(writeBuffer));
						break;
					}
				}
				fclose(file);
				file = NULL;
			}
			
		}   else {
				bzero(writeBuffer, MAX);
				strcpy(writeBuffer, "400 Wrong Syntax");
				write(confd, writeBuffer, strlen(writeBuffer));
		}
	}
	return NULL;
}


void* handlingFunction(void* args) {
	while(1) {
		int *client;
		pthread_mutex_lock(&mutex);
		if ((client = dequeue()) == NULL)
		{
			/* code */
			pthread_cond_wait(&condition_check, &mutex);
			client = dequeue();
		}
		
		pthread_mutex_unlock(&mutex);
		if (client != NULL)
		{
			/* code */
			commute(client);
		}
	}
};
int main(){
	int sockfd, confd, len;

	// Handling quá tải server khi với mỗi client thì tạo 1 thread
	// Tạo ra vùng chứa thread với số lượng limit
	// Update lại vùng khi có client connect và disconnect
	for (int i = 0; i < thread_handling; ++i)
	{
		/* code */
		pthread_create(&sourceOfThread[i], NULL,handlingFunction,NULL);

	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		/* code */
		printf("Fail to create \n");
	} else {
		printf("Create successfull ! \n");
	}

	struct sockaddr_in serveras, cli;
	bzero(&serveras, sizeof(serveras));

	serveras.sin_family = AF_INET;
	serveras.sin_addr.s_addr = htonl(INADDR_ANY);
	serveras.sin_port = htons(PORT);

	if ((bind(sockfd,(SA*)&serveras, sizeof(serveras))) != 0)
	{
	 	/* code */
	 	printf("Binding failed \n");
	 	exit(0);
	} else {
	 	printf("Binding success ! \n");
	}

	if (listen(sockfd,Server_handle) != 0)
	{
		/* code */
		printf("Listening failed ! \n");
	} else {
		printf("Server Listening .. \n");
	}
	len = sizeof(cli);

	while (1) {
		printf("Waiting for connection ...\n");
		confd = accept(sockfd, (SA*)&cli, &len);
		if (confd < 0) {
		/* code */
			printf("Accept Failed \n");
		} else {
			printf("Accept Client \n");
		} 
		
		int *client = malloc(sizeof(int));
		*client = confd;
		// pthread_t t;
		// pthread_create(&t, NULL, commute, client);
		pthread_mutex_lock(&mutex);
		enqueue(client);
		pthread_cond_signal(&condition_check); 
		pthread_mutex_unlock(&mutex);
	}

	
	close(sockfd);
	return 0;
}



