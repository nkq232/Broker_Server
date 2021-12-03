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
#include "sqltest.c"
#include <json.c/json.h>

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
	int escape = 0;
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
		else 
		{
			write(confd, "Syntax Error", sizeof("Syntax Error"));
			printf("Error Input\n");
		}
		if (isClient) {
			//Xu ly giao thuc client
			free(isClient);
			int sign = 0;
			while(1) {
				bzero(readBuffer,MAX);
				read(confd, readBuffer, sizeof(readBuffer));
				if (!sign){
					if (strncmp(readBuffer, "SIGN UP", 7) == 0)
					{
						/* code */
						write(confd, "210 Sign up OK", strlen("210 Sign up OK"));
						printf("Sending to client: 210 Sign up OK");
						bzero(readBuffer,MAX);
						read(confd, readBuffer, sizeof(readBuffer));
						write(confd, "211 Account OK", strlen("211 Account OK"));
						printf("Sending to client: 211 Account OK");


					} else if (strncmp(readBuffer, "SIGN IN", 7) == 0)
					{
						/* code */
						write(confd, "220 Sign in OK", strlen("220 Sign in OK"));
						printf("Sending to client: 220 Sign in OK");
						bzero(readBuffer,MAX);
						read(confd, readBuffer, sizeof(readBuffer));
						write(confd, "221 Account OK", strlen("221 Account OK"));
						printf("Sending to client: 211 Account OK");
					} else {
						write(confd, "Systax error" , strlen("Syntax error"));
						printf("Sending to client: Syntax error");
					}
				} else {
					if (strncmp(readBuffer, "GET LOCATION", 12) == 0)
					{
						/* code */
						write(confd, "223 GET Location OK", strlen("223 GET Location OK"));
						printf("Sending to client: 223 GET Location OK");
						File* file = fopen("location.txt", "w");
						getLocation(fileName);
						
						/* code */
						size_t a;

						fseek(file, 0, SEEK_END);
						int size = ftell(file); 
						fseek(file, 0, SEEK_SET);

						bzero(writeBuffer, MAX);
						snprintf(writeBuffer, sizeof(writeBuffer), "{Filesize: %d}", size); 
						write(confd, writeBuffer, strlen(writeBuffer));
						printf("Sending to client: Filesize : %d", size);

						bzero(writeBuffer, MAX);

						while(1) {
							if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
							{
								/* code */
								writeBuffer[a] = '\0';
								write(confd, writeBuffer,a);
							} else {
								write(confd, "Done", strlen(Done));
								break;
							}
						} 
						remove(file);
					} else if (strncmp(readBuffer, "GET TYPE", 8) == 0)
					{
						/* code */
						write(confd, "224 GET Type Sensor OK", strlen("223 GET Type Sensor OK"));
						printf("Sending to client: 223 GET Location OK");
						File* file = fopen("sensorType.txt", "w");
						getType(file);
						
						/* code */
						size_t a;

						fseek(file, 0, SEEK_END);
						int size = ftell(file); 
						fseek(file, 0, SEEK_SET);

						bzero(writeBuffer, MAX);
						snprintf(writeBuffer, sizeof(writeBuffer), "{Filesize: %d}", size); 
						write(confd, writeBuffer, strlen(writeBuffer));
						printf("Sending to client: Filesize : %d", size);

						bzero(writeBuffer, MAX);	

						while(1) {
							if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
							{
								/* code */
								writeBuffer[a] = '\0';
								write(confd, writeBuffer,a);
							} else {
								write(confd, "Done", strlen(Done));
								break;
							}
						} 
						remove(file);
					} else if (strncmp(readBuffer, "GET INFO REGISTER", 17) == 0)
					{
						/* code */
						write(confd, "230 Info Register OK", strlen("230 Info Register OK"));
						printf("Sending to client: 230 Info Register OK");
						File* file = fopen("infoRegister.txt", "w");
						getLocation(fileName);
						
						/* code */
						size_t a;

						fseek(file, 0, SEEK_END);
						int size = ftell(file); 
						fseek(file, 0, SEEK_SET);

						bzero(writeBuffer, MAX);
						snprintf(writeBuffer, sizeof(writeBuffer), "{Filesize: %d}", size); 
						write(confd, writeBuffer, strlen(writeBuffer));
						printf("Sending to client: Filesize : %d", size);

						bzero(writeBuffer, MAX);

						while(1) {
							if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
							{
								/* code */
								writeBuffer[a] = '\0';
								write(confd, writeBuffer,a);
							} else {
								write(confd, "Done", strlen(Done));
								break;
							}
						} 
						remove(file);
					} else if (strncmp(readBuffer, "GET TYPE", 8) == 0)
					{
						/* code */
						write(confd, "223 GET Location OK", strlen("223 GET Location OK"));
						printf("Sending to client: 223 GET Location OK");
						File* file = fopen("location.txt", "w");
						getLocation(fileName);
						
						/* code */
						size_t a;
						while(1) {
							if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
							{
								/* code */
								writeBuffer[a] = '\0';
								write(confd, writeBuffer,a);
							} else {
								write(confd, "Done", strlen(Done));
								break;
							}
						} 
						remove(file);
					} else if (strncmp(readBuffer, "GET TYPE", 8) == 0)
					{
						/* code */
						write(confd, "223 GET Location OK", strlen("223 GET Location OK"));
						printf("Sending to client: 223 GET Location OK");
						File* file = fopen("location.txt", "w");
						getLocation(fileName);
						
						/* code */
						size_t a;
						while(1) {
							if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
							{
								/* code */
								writeBuffer[a] = '\0';
								write(confd, writeBuffer,a);
							} else {
								write(confd, "Done", strlen(Done));
								break;
							}
						} 
						remove(file);
					} else if (strncmp(readBuffer, "GET TYPE", 8) == 0)
					{
						/* code */
						write(confd, "223 GET Location OK", strlen("223 GET Location OK"));
						printf("Sending to client: 223 GET Location OK");
						File* file = fopen("location.txt", "w");
						getLocation(fileName);
						
						/* code */
						size_t a;
						while(1) {
							if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
							{
								/* code */
								writeBuffer[a] = '\0';
								write(confd, writeBuffer,a);
							} else {
								write(confd, "Done", strlen(Done));
								break;
							}
						} 
						remove(file);
					}

				}
			}
			escape = 1;

		}
		if (isSensor) {
			// Xy ly giao thuc sensor
			free(isSensor);
			read(confd, readBuffer, sizeof(readBuffer));
			printf("Recieving Message From Sensor: %s\n" ,readBuffer);
			write(confd, "501 Info OK", strlen("501 Info OK"));
			printf("Sending to Sensor: 501 Info OK\n");
			bzero(readBuffer, MAX);
			char id[50], type[50], locationID[50], value[50];
			struct json_object *parsed_json;
			struct json_object *id;
			struct json_object *type;
			struct json_object *locationID;
			struct json_object *value;
						
			// read(confd, readBuffer, sizeof(readBuffer));
			// int check1 = 0, check2 = 0, check3 = 0, check4 = 0;
			// int for1 = 0, for2 = 0, for3 = 0, for4 = 0, for_stop = 0;
			// for (int i = 0; i < strlen(readBuffer); ++i)
			// {
			// 	/* code */
			// 	if (readBuffer[i] == 'I' && readBuffer[i+1] == 'D' && for1 == 0)
			// 	{
			// 		/* code */
			// 		check1 = i;
			// 		for1 = 1;
			// 	}
			// 	if (readBuffer[i] == 'T' && for2 == 0)
			// 	{
			// 		/* code */
			// 		check2 = i;
			// 		for2 = 1;
			// 	}
			// 	if (readBuffer[i] == 'L' && readBuffer[i+1] == 'o' && readBuffer[i+2] == 'c' && readBuffer[i+3] == 'a' && for3 == 0)
			// 	{
			// 		/* code */
			// 		check3 = i;
			// 		for3 = 1;
			// 	}
			// 	if (readBuffer[i] == 'V' && readBuffer[i+1] == 'a' && readBuffer[i+2] == 'l' && readBuffer[i+3] == 'u' && for4 == 0)
			// 	{
			// 		/* code */
			// 		check4 = i;
			// 		for4 = 1;
			// 	}

			// }
			// char id[50], type[50], locationID[50], value[50];
			// for (int i = 0; i < strlen(readBuffer); ++i)
			// {
			// 	/* code */
			// 	if (i >= check1 && i < (check2 - 2))
			// 	{
			// 		/* code */
			// 		strcat(id, readBuffer[i]);
			// 	}
			// 	if (i >= check2 && i < (check3 - 2))
			// 	{
			// 		/* code */
			// 		strcat(type, readBuffer[i]);
			// 	}
			// 	if (i >= check3 && i < (check4 - 2))
			// 	{
			// 		/* code */
			// 		strcat(locationID, readBuffer[i]);
			// 	}
			// 	if (i >= check4)
			// 	{
			// 		/* code */
			// 		if (readBuffer[i] == '\n')
			// 		{
			// 			/* code */
			// 			for_stop = 1;

			// 		}
			// 		if (for_stop)
			// 		{
			// 			/* code */
			// 			break;
			// 		}
			// 		strcat(value, readBuffer[i]);
			// 	}

			// }
			setInfo(id, type, locationID, value);
			escape = 1;

		}
		if (escape)
		{
			/* code */
			break;
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
