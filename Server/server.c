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
#include "sqltest.c"



#define MAX 1024
#define PORT 8086
#define SA struct sockaddr
#define Server_handle 100
#define thread_handling 5
#define timeUpdate 1000

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
		printf("Receiving Message: %s\n" ,readBuffer);
		if(strncmp(readBuffer, "CLIENT HELO Broker", 18) == 0){
			isClient = 1;
			if(write(confd, "200 Hello Client", strlen("200 Hello Client"))){
				printf("Sending to client: 200 Hello Client\n");
			} else {
				printf("Sending 200 Hello Client error\n");
				break;
			}
			
		} 
		
		else if(strncmp(readBuffer, "SENSOR HELO Broker", 18) == 0){
			isSensor = 1;
			if(write(confd, "500 Hello Sensor", strlen("500 Hello Sensor"))){
				printf("Sending to sensor: 500 Hello Sensor\n");
			} else {
				printf("Sending 500 Hello Sensor error\n");
				break;
			}
			
		} else {
			if(write(confd, "Syntax error", strlen("Syntax error"))){
				printf("Sending : Syntax error\n");
			} else {
				printf("Cannot Send Syntax error \n");
				break;
			}
			
		}
		
		if (isClient) {
			//Xu ly giao thuc client
			//free(isClient);
			int sign = 0;
			char userClientId[MAX];
			int myUserId = -1;
			while(1){
				bzero(readBuffer,MAX);
				if(read(confd, readBuffer, sizeof(readBuffer))){
					printf("From Client: %s\n",readBuffer);
				} else {
					printf("Error in Big While(1) : \n");
					break;
				}
				if (strncmp(readBuffer, "QUIT", 4) == 0){
					if (write(confd, "999 bye", strlen("999 bye"))){
						printf("Sending to client: 999 bye\n");
					} else {
						printf("Sending 999 bye error\n");
						break;
					}
					break;
				} else {
					if (!sign){
						if (strncmp(readBuffer, "SIGN UP", 7) == 0){
							
							if (write(confd, "210 Sign up OK", strlen("210 Sign up OK"))){
								printf("Sending to client: 210 Sign up OK\n");
							} else {
								printf("Sending 210 Sign up error\n");
								break;
							} 
							
							
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in SIGN UP 1: \n");
								break;
							}

							if(write(confd, "211 Account OK", strlen("211 Account OK"))) {
								printf("Sending to client: 211 Account OK\n");
							} else {
								printf("Sending 211 Account error\n");
								break;
							}
							

							bzero(readBuffer, MAX);
							if (read(confd, readBuffer, sizeof(readBuffer))){
								/* code */
								printf("From Client: %s\n", readBuffer);
							} else {
								printf("Error in SIGN UP 2: \n");
								break;
							}

							struct json_object *parsed_json;
							struct json_object *username;
							struct json_object *password;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "Username", &username);
							json_object_object_get_ex(parsed_json, "Password", &password);

							if(signUp(json_object_get_string(username), json_object_get_string(password)) == 1) {
								if(write(confd, "212 Register Success", strlen("212 Register Success"))){
									printf("Sending to client: 212 Register Success\n");
								} else {
									printf("Sending 212 Register error\n");
									break;
								}
								
							} else {
								if(write(confd, "413 Username Existed", strlen("413 Username Existed"))) {
									printf("Sending to client: 413 Username Existed\n");
								} else {
									printf("Sending 413 Username Existed error\n");
									break;
								}
							}
						} else if (strncmp(readBuffer, "SIGN IN", 7) == 0){
							/* code */
							if(write(confd, "220 Sign in OK", strlen("220 Sign in OK"))){
								printf("Sending to client: 220 Sign in OK\n");
							} else {
								printf("Sending 220 Sign in OK error\n");
								break;
							}
							
							bzero(readBuffer,MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in SIGN IN 1: \n");
								break;
							}

							if(write(confd, "221 Account OK", strlen("221 Account OK"))){
								printf("Sending to client: 211 Account OK\n");
							} else {
								printf("Sending 211 Account error\n");
								break;
							}
							

							bzero(readBuffer, MAX);
							if (!read(confd, readBuffer, sizeof(readBuffer))){
								/* code */
								printf("Error in SIGN IN 2: \n");
								break;
							}

							struct json_object *parsed_json;
							struct json_object *username;
							struct json_object *password;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "Username", &username);
							json_object_object_get_ex(parsed_json, "Password", &password);
							strcpy(userClientId, signIn(json_object_get_string(username), json_object_get_string(password)));
							myUserId = cvtChar2Int(userClientId);
							if (strcmp(userClientId, "-1") != 0)
							{
								printf("UserId2: %s\n", userClientId);
								/* code */
								if(write(confd, "222 Login Success", strlen("222 Login Success"))){
									printf("Sending to client: 222 Login Success\n");
								} else {
									printf("Sending 218 Login Success error\n");
									break;
								}
								
								sign = 1;
							} else {
								if(write(confd, "421 Wrong Account", strlen("421 Wrong Account"))){
									printf("Sending to client: 421 Wrong Account");
								} else {
									printf("Sending Wrong Account error\n");
									break;
								}
								
							}
						} else {
							if(write(confd, "Systax error" , strlen("Syntax error"))){
								printf("Sending to client: Syntax error");
							} else {
								printf("Cannot Send Syntax error\n");
								break;
							}	
						}
					} else {
						if (strncmp(readBuffer, "SIGN OUT", 8) == 0) {
							/* code */
							sign = 0;
							if(write(confd, "230 Sign out OK", strlen("230 Sign out OK"))){
								printf("Sending to client: 230 Sign out OK");
							} else {
								printf("Sending 230 Sign out error\n");
								break;
							}
							
						} else if (strncmp(readBuffer, "GET LOCATION", 12) == 0) {
							/* code */
							if(write(confd, "240 GET Location OK", strlen("240 GET Location OK"))){
								printf("Sending to client: 240 GET Location OK");
							} else {
								printf("Sending 240 GET Location error\n");
								break;
							}
							
							FILE* file = fopen("location.json", "w+");
							if(getLocation(file) == 1){
								printf("Get file for GET LOCATION success\n");
							} else {
								printf("Error when getting file for GET LOCATION\n");
								break;
							};

							
							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET Location 1: \n");
								break;
							}
							
							/* code */
							size_t a;

							fseek(file, 0, SEEK_END);
							int size = ftell(file); 
							fseek(file, 0, SEEK_SET);

							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							if(write(confd, writeBuffer, strlen(writeBuffer))){
								printf("Sending to client: Filesize : %d", size);
							} else {
								printf("Sending file size error\n");
								break;
							}
							

							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET Location 2: \n");
								break;
							}

							bzero(writeBuffer, MAX);


							while(1) {
								if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
								{
									/* code */
									writeBuffer[a] = '\0';
									int n = write(confd, writeBuffer,a);
									if (!n){
										printf("Error when sending file in GET LOCATION");
										break;
									}
								} else {
									
									break;
								}
							} 
							remove(file);
							printf("Sending file to client complete\n");
							// if (sendingFile(confd, &file, &readBuffer, &writeBuffer, "GET LOCATION")){
							// 	remove(file);
							// 	printf("Sending file to client complete\n");
							// } else {
							// 	printf("Sending file error at GET LOCATION\n");
							// }
						} else if (strncmp(readBuffer, "GET TYPE", 8) == 0){
							/* code */
							if(write(confd, "241 GET Type Sensor OK", strlen("241 GET Type Sensor OK"))){
								printf("Sending to client: 241 GET Type Sensor OK");
							} else {
								printf("Sending 241 GET Type Sensor error\n");
								break;
							}

							FILE* file = fopen("sensorType.json", "w+");
							if(getType(file) == 1){
								printf("Get file for getType success\n");
							} else {
								printf("Error when get file for GET TYPE\n");
								break;
							};

							
							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET TYPE 1: \n");
								break;
							}

							/* code */
							size_t a;

							fseek(file, 0, SEEK_END);
							int size = ftell(file); 
							fseek(file, 0, SEEK_SET);

							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							if(write(confd, writeBuffer, strlen(writeBuffer))){
								printf("Sending to client: Filesize : %d", size);
							} else {
								printf("Sending file size error\n");
								break;
							}
							

							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET TYPE 2 : \n");
								break;
							}

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
							// if (sendingFile(confd, &file, &readBuffer, &writeBuffer, "GET TYPE")){
							// 	remove(file);
							// 	printf("Sending file to client complete\n");
							// } else {
							// 	printf("Sending file error at GET TYPE\n");
							// }
						} else if (strncmp(readBuffer, "GET INFO REGISTER", 17) == 0){
							/* code */
							if(write(confd, "242 Info Register OK", strlen("242 Info Register OK"))){
								printf("Sending to client: 242 Info Register OK");
							} else {
								printf("Sending 242 Info Register error\n");
								break;
							}
							
							FILE* file = fopen("infoRegister.json", "w+");

							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET INFO REGISTER 1 : \n");
								break;
							}

							struct json_object *parsed_json;
							struct json_object *locationID;
							
							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);

							
							if(getTypeByRegister(file,myUserId ,json_object_get_string(locationID)) == 1){
								printf("Get file for GET INFO REGISTER success\n");
							} else {
								printf("GET file for GET INFO REGISTER error\n");
								break;
							};
							
							/* code */
							size_t a;

							fseek(file, 0L, SEEK_END);
							int size = ftell(file); 
							rewind(file);
							
							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							if(write(confd, writeBuffer, strlen(writeBuffer))){
								printf("Sending to client: Filesize : %d", size);
							} else {
								printf("Sending file size error\n");
								break;
							}
							

							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET INFO REGISTER 2 : \n");
								break;
							}

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
							// if (sendingFile(confd, &file, &readBuffer, &writeBuffer, "GET INFO REGISTER")){
							// 	remove(file);
							// 	printf("Sending file to client complete\n");
							// } else {
							// 	printf("Sending file error at GET INFO REGISTER\n");
							// }
						} else if (strncmp(readBuffer, "ADD REGISTER", 12) == 0){
							/* code */
							if(write(confd, "243 Add register OK", strlen("243 Add register OK"))){
								printf("Sending to client: 243 Add register OK");
							} else {
								printf("Sending 243 Add register error\n");
								break;
							}
							
							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in ADD REGISTER 1 : \n");
								break;
							}

							struct json_object *parsed_json;
							struct json_object *locationID;
							struct json_object *typeID;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);
							json_object_object_get_ex(parsed_json, "TypeId", &typeID);
							if(registerInfo(myUserId, json_object_get_string(locationID), json_object_get_string(typeID)) == 1) {
								write(confd, "244 Add Success", strlen("244 Add Success"));
								printf("Sending to client: 244 Add Success");
							} else {
								printf("Add register error\n");
							}
							
							/* code */
						} else if (strncmp(readBuffer, "DELETE REGISTER", 15) == 0){
							/* code */
							if(write(confd, "245 Delete register OK", strlen("245 Delete register OK"))){
								printf("Sending to client: 245 Delete register OK");
							} else {
								printf("Sending 245 Delete register error\n");
								break;
							}
							
							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in DELETE REGISTER 1 : \n");
								break;
							}

							struct json_object *parsed_json;
							struct json_object *locationID;
							struct json_object *typeID;

							parsed_json = json_tokener_parse(readBuffer);

							json_object_object_get_ex(parsed_json, "LocationId", &locationID);
							json_object_object_get_ex(parsed_json, "TypeId", &typeID);
							if(deleteRegisterInfo(myUserId, json_object_get_string(locationID), json_object_get_string(typeID)) == 1) {
								
								if(write(confd, "246 Delete Success", strlen("246 Delete Success"))){
									printf("Sending to client: 246 Delete Success");
								} else {
									printf("Sending 246 Delete error\n");
									break;
								}
								
							} else {
								printf("Delete register error\n");
							}	
						} else if (strcmp(readBuffer, "GET INFO SENSOR") == 0){
							/* code */
							if(write(confd, "251 Get Info Sensor OK", strlen("251 Get Info Sensor OK"))){
								printf("Sending to client: 251 Get Info Sensor OK");
							} else {
								printf("Sending 251 Get Info Sensor error\n");
								break;
							}
							
							
							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET INFO SENSOR 1 : \n");
								break;
							}

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

							FILE* file = fopen("getInformationSensor.json", "w+");

							switch(json_object_get_string(typeTime)[0])
							{
								case 'D':
									getInfoByDay(file, json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date));
									break;
								case 'M':
									getInfoByMonth(file, json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date));
									break;
								default:
									getInfoByYear(file, json_object_get_string(typeID), json_object_get_string(locationID), json_object_get_string(date));						
							}
							size_t a;

							fseek(file, 0, SEEK_END);
							int size = ftell(file); 
							fseek(file, 0, SEEK_SET);

							bzero(writeBuffer, MAX);
							snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
							if(write(confd, writeBuffer, strlen(writeBuffer))){
								printf("Sending to client: Filesize : %d", size);
							} else {
								printf("Sending Filesize error\n");
								break;
							}
							

							bzero(readBuffer, MAX);
							if(read(confd, readBuffer, sizeof(readBuffer))){
								printf("From Client: %s\n",readBuffer);
							} else {
								printf("Error in GET INFO SENSOR 2 : \n");
								break;
							}

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
							// if (sendingFile(confd, &file, &readBuffer, &writeBuffer, "GET INFO SENSOR")){
							// 	remove(file);
							// 	printf("Sending file to client complete\n");
							// } else {
							// 	printf("Sending file error at GET INFO SENSOR\n");
							// }
						} else if (strncmp(readBuffer, "GET INFO SENSOR NOW", 19) == 0) {
								/**
								* Tao file json
								* Lay mang type getTypeByUser(arr, &len, userID, locationId);
								* Duyet mang type lay value, ghi ra file json
								* Gui file json
								*/
								if(write(confd, "252 Get Info Sensor now OK", strlen("252 Get Info Sensor now OK"))){
									printf("Sending to client: 252 Get Info Sensor now OK\n");
								} else {
									printf("Sending 252 Get Info Sensor now error\n");
									break;
								}
								
								bzero(readBuffer, MAX);
								if(read(confd, readBuffer, sizeof(readBuffer))){
									printf("From Client: %s\n",readBuffer);
								} else {
									printf("Error in get info sensor now 1 : \n");
									break;
								}

								struct json_object *parsed_json;
								struct json_object *locationId;

								parsed_json = json_tokener_parse(readBuffer);
								json_object_object_get_ex(parsed_json, "LocationId", &locationId);

								FILE *file = fopen("infoNow.json", "w+");
								int typeList[SENSOR_MAX];
								int len;

								int locationIdInt = cvtChar2Int(json_object_get_string(locationId));
								
								if(getTypeByUser(typeList, &len, myUserId, json_object_get_string(locationId)) < 0) {
									len = 0;
								}
								printf("\nLen: %d\n", len);

								fprintf(file,"[\n");
								for (int i = 0; i < len; i ++) {
									char * tempVlue = (strcmp(current_values[locationIdInt][typeList[i]],"") == 0) ? current_values[locationIdInt][typeList[i]] : "\"...\"";
									printf("{\"TypeID\": %d, \"Value\": %s}", typeList[i], tempVlue);
									fprintf(file, "{\"TypeID\": %d, \"Value\": %s}", typeList[i], tempVlue);
									if (i < len - 1) fprintf(file, ",\n");
 								}
								fprintf(file,"\n]");

								size_t a;

								fseek(file, 0, SEEK_END);
								int size = ftell(file); 
								fseek(file, 0, SEEK_SET);

								bzero(writeBuffer, MAX);
								snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
								if(write(confd, writeBuffer, strlen(writeBuffer))){
									printf("Sending to client: Filesize : %d\n", size);
								} else {
									printf("Sending Filesize error\n");
									break;
								}
								

								bzero(readBuffer, MAX);
								if(read(confd, readBuffer, sizeof(readBuffer))){
									printf("From Client: %s\n",readBuffer);
								} else {
									printf("Error in GET INFO SENSOR NOW: \n");
									break;
								}

								bzero(writeBuffer, MAX);

								while(1) {
									if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
									{
										/* code */
										writeBuffer[a] = '\0';
										int n = write(confd, writeBuffer,a);
										if (!n){
											break;
										}
									} else {
										break;
									}
								} 
								remove(file);
								printf("Sending file to client complete\n");
								// if (sendingFile(confd, &file, &readBuffer, &writeBuffer, "GET INFO SENSOR NOW")){
								// 	remove(file);
								// 	printf("Sending file to client complete\n");
								// } else {
								// 	printf("Sending file error at GET INFO SENSOR NOW\n");
								// }
							}
						else {
							if(write(confd, "Syntax error", strlen("Syntax error"))){
								printf("Sending to client: Syntax error");
							} else {
								printf("Cannot Send Syntax error\n");
								break;
							}
							
						}
					}
				}
			}
			escape = 1;
			// communicateWithClient(confd, &readBuffer, &writeBuffer, &escape, isClient);
		}
		else if (isSensor) {
			// Xy ly giao thuc sensor
			while(1) {
				bzero(readBuffer, MAX);
				if(read(confd, readBuffer, sizeof(readBuffer))){
					printf("From Sensor: %s\n",readBuffer);
				} else {
					printf("Error in sensor \n");
					break;
				}
				
				if(write(confd, "501 Info OK", strlen("501 Info OK"))){
					printf("Sending to Sensor: 501 Info OK\n");
				} else {
					printf("Sending 501 Info error\n");
					break;
				}
				

				bzero(readBuffer, MAX);
				if(read(confd, readBuffer, sizeof(readBuffer))){
					printf("From Client: %s\n",readBuffer);
				} else {
					printf("400 Get info error\n");
					break;
				}

				struct json_object *parsed_json;
				struct json_object *type;
				struct json_object *locationID;
				struct json_object *value;
							

				parsed_json = json_tokener_parse(readBuffer);

				json_object_object_get_ex(parsed_json, "TypeID", &type);
				json_object_object_get_ex(parsed_json, "LocationID", &locationID);
				json_object_object_get_ex(parsed_json, "Value", &value);
				int x = cvtChar2Int(json_object_get_string(locationID));
				int y = cvtChar2Int(json_object_get_string(type));

				// printf("\nLId: %d, TId: %d\n", x, y);
				current_values[x][y] = json_object_get_string(value);

				if(write(confd, "502 Get info success", strlen("502 Get info success"))){
					printf("Sending to sensor: 502 Get info success");
				} else {
					printf("Sending 502 Get info error\n");
					break;
				}

			}
			escape = 1;
			// communicateWithSensor(confd, &readBuffer, &writeBuffer, &escape);
		} 
		if (escape){
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

	//Update Database
	

	// Handling quá tải server khi với mỗi client thì tạo 1 thread
	// Tạo ra vùng chứa thread với số lượng limit
	// Update lại vùng khi có client connect và disconnect
	initDataTable();
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




int sendingFile(int confd, FILE* file, char* readBuffer, char* writeBuffer, char* errorDetection) {
	size_t a;
	fseek(file, 0, SEEK_END);
	int size = ftell(file); 
	fseek(file, 0, SEEK_SET);

	bzero(writeBuffer, MAX);
	snprintf(writeBuffer, sizeof(writeBuffer), "{\"Filesize\": %d}", size); 
	write(confd, writeBuffer, strlen(writeBuffer));
	printf("Sending to client: Filesize : %d", size);

	bzero(readBuffer, MAX);
	if(read(confd, readBuffer, sizeof(readBuffer))){
		printf("From Client: %s\n",readBuffer);
	} else {
		printf("Error in %s \n", errorDetection);
		return -1;
	}

	bzero(writeBuffer, MAX);

	while(1) {
		if ((a = fread(writeBuffer, sizeof(char), sizeof(writeBuffer), file)) > 0)
		{
			/* code */
			writeBuffer[a] = '\0';
			int n = write(confd, writeBuffer,a);
			if (!n){
				printf("Error sending file in %s\n", errorDetection);
				return -1;
			}
		} else {
			remove(file);
			printf("Sending file to client complete\n");
			return 1;
		}
	}
	return 1; 
}
void communicateWithClient(int confd, char* readBuffer, char* writeBuffer, int escape, int isClient){
}

void communicateWithSensor(int confd, char* readBuffer, char* writeBuffer, int escape){
}