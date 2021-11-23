#include <mysql.h>
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


int main(){
	MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "weather_server", "12345678",
          "Weather", 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if(mysql_query(con, "select * from Register r join User u on r.userID = u.userID where u.userID = 1;")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for(int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    mysql_free_result(result);


    mysql_close(con);

	return 0;
}



