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


void  *communicate(void * client){
	int confd = *((int*)client);
	free(client);

	char readBuffer[MAX], writeBuffer[MAX];
	int n;
	int check_read;
	short isClient = 0;
	short isSensor = 0;
	while (1) {
		bzero(readBuffer, MAX);
		read(confd, readBuffer, sizeof(readBuffer));
		printf("Recieving Message: %s\n" ,readBuffer);
		if(strncmp(readBuffer, "CLIENT HELO Broker", 18) == 0) {
			isClient = 1;
			write(confd, "200 Hello Client", strlen("200 Hello Client"));
			printf("Sending to client: 200 Hello Client\n");
		}
		else if(strncmp(readBuffer, "SENSOR HELO Broker", 18) == 0) {
			isSensor = 1;
			write(confd, "500 Hello Sensor", strlen("500 Hello Sensor"));
			printf("Sending to client: 500 Hello Sensor\n");
		}
		else if (strncmp(readBuffer, "QUIT", 4) == 0)
		{
			write(confd, "500 bye", sizeof("500 bye"));
			printf("Sending to client: 500 bye\n");
			break;
		}
		else 
		{
			write(confd, "Syntax Error", sizeof("Syntax Error"));
			printf("Error Input\n");
		}
		if (isClient) {
			//Xu ly giao thuc client
			free(isClient);
			free(isSensor);

		}
		if (isSensor) {
			// Xy ly giao thuc sensor
			free(isSensor);
			free(isClient);
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
			communicate(client);
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
		// pthread_create(&t, NULL, communicate, client);
		pthread_mutex_lock(&mutex);
		enqueue(client);
		pthread_cond_signal(&condition_check); 
		pthread_mutex_unlock(&mutex);
	}

	
	close(sockfd);
	return 0;
}




//Dung cho client, tham so dang de trong
void checkUsername(char *username);
void checkAccount(char *username, char *password);
void registerInfo(char *username, short locationID, short sensorIDList[]);

void getInfoImmediately(char* fileName, char *type, short locationID);
void getInfoByDay(char* fileName, char *type, short locationID, char *start, char *end);
void getInfoByMonth(char* fileName, char *type, short localocationIDtion, char *start, char *end);
void getInfoByYear(char* fileName, char *type, short locationID, char *start, char *end);

//Dung cho sensor, tham so dang de trong
void setInfo(char *type, short locationID, char *timeType, float value);


void communicateWithClient(void * client);
void communicateWithSensor(void * sensor);
