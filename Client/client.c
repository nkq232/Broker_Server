#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX 1024
#define PORT 8082
#define SA struct sockaddr
void commute(int sockfd)
{
    char writeBuffer[MAX];
    char readBuffer[MAX];
    int check_read;
    FILE* file = NULL;
    while(1) {
        bzero(writeBuffer, sizeof(writeBuffer));
        printf("Enter message : ");
        scanf("%[^\n]%*c", writeBuffer); 
        write(sockfd, writeBuffer, sizeof(writeBuffer));
        
        bzero(readBuffer, sizeof(readBuffer));
        read(sockfd, readBuffer, sizeof(readBuffer));
        printf("From Server : %s\n", readBuffer);
        
        if ((strncmp(writeBuffer, "QUIT", 4) == 0))
        {
            /* code */
            write(sockfd, "Bye", sizeof("Bye"));
            break;
        }
        if (strncmp(writeBuffer, "DOWNLOAD", 8) == 0)
        {
            /* code */
            printf("Enter filename : ");
            bzero(writeBuffer, sizeof(writeBuffer));
            scanf("%[^\n]%*c", writeBuffer);
            write(sockfd, writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            read(sockfd, readBuffer, sizeof(readBuffer));
            printf("From Server: %s\n", readBuffer);
            if (strncmp(readBuffer, "305 File Not Exists", 19) == 0)
            {
                /* code */
                
            } else {
                file = fopen(writeBuffer, "wb");
                writeBuffer[strlen(writeBuffer) - 1] = '\0';
                
                strcpy(writeBuffer, "Ok");
                write(sockfd, writeBuffer, strlen(writeBuffer));

                read(sockfd, readBuffer, sizeof(readBuffer));
                printf("From Server: %s\n", readBuffer);

                while(1) {
                    strcpy(writeBuffer, "OK");
                    write(sockfd, writeBuffer, strlen(writeBuffer));

                    check_read = read(sockfd, readBuffer, sizeof(readBuffer));
                    readBuffer[check_read] = '\0';
                    if (strcmp(readBuffer, "320 End File") == 0)
                    {
                        /* code */
                        printf("From Server: %s\n", readBuffer);
                        break;
                    }
                    fwrite(readBuffer, 1, check_read, file);
                }
                fclose(file);
            file = NULL;
            }
            

        }
    }
}
   
int main(int argc, char* args[])
{
    int sockfd, confd;
    struct sockaddr_in serveras, cli;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&serveras, sizeof(serveras));
   
    serveras.sin_family = AF_INET;
    serveras.sin_port = htons(PORT);
    if (inet_pton(serveras.sin_family, args[1], &serveras.sin_addr) < 0) {
        perror("error set server address");
        return 1;
    } 
    
    if (connect(sockfd, (SA*)&serveras, sizeof(serveras)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
   
   
    commute(sockfd);
   
    
    close(sockfd);
}
