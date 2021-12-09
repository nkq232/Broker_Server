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
#include <json-c/json.h>

#define MAX 1024
#define PORT 8082
#define SA struct sockaddr
#define Server_handle 100
#define thread_handling 5

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
			char userClientId[MAX];
			while(1) {
				bzero(readBuffer,MAX);
				read(confd, readBuffer, sizeof(readBuffer));
				if (strncmp(readBuffer, "QUIT", 4) == 0)
				{
						/* code */
						write(confd, "999 bye", strlen("999 bye"));
						printf("Sending to client: 999 bye");
						break;
				} else {
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

							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							struct json_object *parsed_json;
							struct json_object *username;
							struct json_object *password;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "Username", &username);
							json_object_object_get_ex(parsed_json, "Password", &password);
							if(signUp(json_object_get_string(username), json_object_get_string(password)) == 1) {
								write(confd, "219 Register Success", strlen("219 Register Success"));
								printf("Sending to client: 212 Register Success");
							} else {
								write(confd, "413 Username Existed", strlen("413 Username Existed"));
								printf("Sending to client: 413 Username Existed");
							}

						} else if (strncmp(readBuffer, "SIGN IN", 7) == 0)
						{
							/* code */
							write(confd, "220 Sign in OK", strlen("220 Sign in OK"));
							printf("Sending to client: 220 Sign in OK");
							bzero(readBuffer,MAX);
							read(confd, readBuffer, sizeof(readBuffer));
							write(confd, "221 Account OK", strlen("221 Account OK"));
							printf("Sending to client: 211 Account OK");

							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							struct json_object *parsed_json;
							struct json_object *username;
							struct json_object *password;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "Username", &username);
							json_object_object_get_ex(parsed_json, "Password", &password);
							strcpy(userClientId, signIn(json_object_get_string(username), json_object_get_string(password)));

							if (userClientId != '-1')
							{
								/* code */
								write(confd, "222 Login Success", strlen("222 Login Success"));
								printf("Sending to client: 218 Login Success");
							} else {
								write(confd, "421 Wrong Account", strlen("421 Wrong Account"));
								printf("Sending to client: 407 Wrong Account");
							}

							
						} else {
							write(confd, "Systax error" , strlen("Syntax error"));
							printf("Sending to client: Syntax error");
						}
					} else {
						if (strncmp(readBuffer, "SIGN OUT", 8) == 0)
						{
							/* code */
							sign = 0;
							write(confd, "230 Sign out OK", strlen("230 Sign out OK"));
							printf("Sending to client: 230 Sign out OK");
						} else if (strncmp(readBuffer, "GET LOCATION", 12) == 0)
						{
							/* code */
							write(confd, "240 GET Location OK", strlen("240 GET Location OK"));
							printf("Sending to client: 240 GET Location OK");
							FILE* file = fopen("location.json", "w");
							getLocation(file);
							
							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));
							/* code */
							size_t a;

							fseek(file, 0, SEEK_END);
							int size = ftell(file); 
							fseek(file, 0, SEEK_SET);

							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							write(confd, writeBuffer, strlen(writeBuffer));
							printf("Sending to client: Filesize : %d", size);

							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							bzero(writeBuffer, MAX);

							while(1) {
								if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
								{
									/* code */
									writeBuffer[a] = '\0';
									write(confd, writeBuffer,a);
								} else {
									
									break;
								}
							} 
							remove(file);
						} else if (strncmp(readBuffer, "GET TYPE", 8) == 0)
						{
							/* code */
							write(confd, "241 GET Type Sensor OK", strlen("241 GET Type Sensor OK"));
							printf("Sending to client: 241 GET Location OK");
							FILE* file = fopen("sensorType.json", "w");
							getType(file);
							
							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							/* code */
							size_t a;

							fseek(file, 0, SEEK_END);
							int size = ftell(file); 
							fseek(file, 0, SEEK_SET);

							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							write(confd, writeBuffer, strlen(writeBuffer));
							printf("Sending to client: Filesize : %d", size);

							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							bzero(writeBuffer, MAX);	

							while(1) {
								if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
								{
									/* code */
									writeBuffer[a] = '\0';
									write(confd, writeBuffer,a);
								} else {
									
									break;
								}
							} 
							remove(file);
						} else if (strncmp(readBuffer, "GET INFO REGISTER", 17) == 0)
						{
							/* code */
							write(confd, "242 Info Register OK", strlen("242 Info Register OK"));
							printf("Sending to client: 242 Info Register OK");
							FILE* file = fopen("infoRegister.json", "w");
							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							struct json_object *parsed_json;
							struct json_object *locationID;
							
							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);
							
							getTypeByLocation(file, json_object_get_string(locationID));
							
							/* code */
							size_t a;

							fseek(file, 0, SEEK_END);
							int size = ftell(file); 
							fseek(file, 0, SEEK_SET);
							
							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							write(confd, writeBuffer, strlen(writeBuffer));
							printf("Sending to client: Filesize : %d", size);

							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							bzero(writeBuffer, MAX);

							while(1) {
								if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
								{
									/* code */
									writeBuffer[a] = '\0';
									write(confd, writeBuffer,a);
								} else {
									
									break;
								}
							} 
							remove(file);
						} else if (strncmp(readBuffer, "ADD REGISTER", 12) == 0)
						{
							/* code */
							write(confd, "231 Add register OK", strlen("231 Add register OK"));
							printf("Sending to client: 231 Add register OK");
							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							struct json_object *parsed_json;
							struct json_object *locationID;
							struct json_object *typeID;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);
							json_object_object_get_ex(parsed_json, "TypeId", &typeID);
							if(registerInfo(userClientId, json_object_get_string(locationID), json_object_get_string(typeID))) {
								write(confd, "244 Add Success", strlen("244 Add Success"));
								printf("Sending to client: 244 Add Success");
							}
							
							/* code */
						} else if (strncmp(readBuffer, "DELETE REGISTER", 15) == 0)
						{
							/* code */
							write(confd, "245 Delete register OK", strlen("245 Delete register OK"));
							printf("Sending to client: 245 Delete register OK");
							
							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							struct json_object *parsed_json;
							struct json_object *locationID;
							struct json_object *typeID;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);
							json_object_object_get_ex(parsed_json, "TypeId", &typeID);
							if(deleteRegisterInfo(userClientId, json_object_get_string(locationID), json_object_get_string(typeID))) {
								write(confd, "246 Delete Success", strlen("246 Delete Success"));
								printf("Sending to client: 246 Delete Success");
							}
							
						} else if (strncmp(readBuffer, "GET INFO SENSOR", 15) == 0)
						{
							/* code */
							write(confd, "251 Get Info Sensor OK", strlen("251 Get Info Sensor OK"));
							printf("Sending to client: 251 Get Info Sensor OK");
							
							
							
							bzero(readBuffer, MAX);
							read(confd, readBuffer, sizeof(readBuffer));

							struct json_object *parsed_json;
							struct json_object *locationID;
							struct json_object *typeID;
							struct json_object *date;
							struct json_object *typeTime;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);
							json_object_object_get_ex(parsed_json, "TypeId", &typeID);
							json_object_object_get_ex(parsed_json, "Date", &date);
							json_object_object_get_ex(parsed_json, "TypeTime", &typeTime);

							
							if (typeTime == 'Now')
							{
								/* code */
								char result[MAX];
								strcpy(result,getInfoByNow(json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date)));
								bzero(writeBuffer, MAX);
								snprintf(writeBuffer, sizeof(writeBuffer), "{\"Value\": %s}", result); 
								write(confd, writeBuffer, strlen(writeBuffer));
								free(result);
							} else {

								FILE* file = fopen("getInformationSensor.json", "w");

								switch(*(json_object_get_string(typeTime) + 9))
								{
									case 'D':
										getInfoByDay(file, json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date));
										break;
									case 'M':
										getInfoByMonth(file, json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date));
										break;
									case 'Y':
										getInfoByYear(file, json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date));
										break;
									default:
										
								}
								/* code */	

								size_t a;

								fseek(file, 0, SEEK_END);
								int size = ftell(file); 
								fseek(file, 0, SEEK_SET);

								bzero(writeBuffer, MAX);
								snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
								write(confd, writeBuffer, strlen(writeBuffer));
								printf("Sending to client: Filesize : %d", size);

								bzero(readBuffer, MAX);
								read(confd, readBuffer, sizeof(readBuffer));

								bzero(writeBuffer, MAX);

								while(1) {
									if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
									{
										/* code */
										writeBuffer[a] = '\0';
										write(confd, writeBuffer,a);
									} else {
										
										break;
									}
								}

								remove(file);
							}
						} else {
							write(confd, "Syntax error", strlen("Syntax error"));
							printf("Sending to client: Syntax error");
						}

					}
				}
			}
			escape = 1;

		}
		if (isSensor) {
			// Xy ly giao thuc sensor
			free(isSensor);
			bzero(readBuffer, MAX);
			read(confd, readBuffer, sizeof(readBuffer));
			printf("Recieving Message From Sensor: %s\n" ,readBuffer);
			write(confd, "501 Info OK", strlen("501 Info OK"));
			printf("Sending to Sensor: 501 Info OK\n");
			bzero(readBuffer, MAX);
			// char id[50], type[50], locationID[50], value[50];
			struct json_object *parsed_json;
			struct json_object *type;
			struct json_object *locationID;
			struct json_object *value;
						
			read(confd, readBuffer, sizeof(readBuffer));

			parsed_json = json_tokener_parse(readBuffer);

			json_object_object_get_ex(parsed_json, "TypeID", &type);
			json_object_object_get_ex(parsed_json, "LocationID", &locationID);
			json_object_object_get_ex(parsed_json, "Value", &value);

			write(confd, "502 Get info success", strlen("502 Get info success"));
			printf("Sending to sensor: 502 Get info success");

			//Chưa có hàm

			if (insertValueFromSensor(json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(value)))
			{
				/* code */
				write(confd, "502 Get info success", strlen("502 Get info success"));
				printf("Sending to sensor: 502 Get info success");
			} else {
				write(confd, "400 Get info error", strlen("400 Get info error"));
				printf("Sending to sensor: 400 Get info error");
			}
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
