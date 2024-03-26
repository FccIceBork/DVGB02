#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define SIZE 4096
#define DELTA 2208988800    //Tiden mellan 1900 och 1970 i sekunder


int main(int argc, char* argv[])
{
    //Om en port ej valts skicka erro och avsluta
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }

    int port = atoi(argv[1]);
    printf("Port: %d\n", port);

    int sockdf;
    char response[30];
    socklen_t size_of_address;  //Allokera minne till adresstrukturen
    struct sockaddr_in serverAddress;
    time_t currentTime;

    /*Skapar en UDP socket*/
    sockdf = socket(AF_INET, SOCK_DGRAM, 0);
    /*Felhantering*/
    if(sockdf < 0) {
        perror("Fel på socket\n");
        exit(1);
    }

    memset(&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(port);

    memset(response, '\0', sizeof(response));
    sendto(sockdf, response, 0, 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); //Data till servern
    printf("Skickade begäran\n");
    recvfrom(sockdf, response, SIZE, 0, (struct sockaddr*)&serverAddress, &size_of_address); //Data från servern
    currentTime = ntohl(*(unsigned long*)response) - DELTA;     //Subtraherar med sekunder mellan 1900->1970 med idag
    printf("Time from server %s\n", ctime(&currentTime));

    return 0;
}