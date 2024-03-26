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
#define DELTA 2208988800 //Tiden mellan 1900 och 1970 i sekunder

int main(int argc, char **argv) {

    //Om en port ej valts skicka erro och avsluta
    if(argc != 2) {
        printf("Format: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    time_t currentTime;

    int sockdf;
    struct sockaddr_in serverAdress, clientAdress;
    char buffer[SIZE];
    socklen_t addrSize; //Allokera minne till adresstrukturen

    /*Skapar en UDP socket*/
    sockdf = socket(AF_INET, SOCK_DGRAM, 0);
    /*Felhantering*/
    if(sockdf < 0) {
        perror("Fel på socket");
        exit(1);
    }

    memset(&serverAdress, '\0', sizeof(serverAdress));
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(port);
    serverAdress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(sockdf, (struct sockaddr*)&serverAdress, sizeof(serverAdress)) < 0) {
        perror("Bind error");
        exit(1);
    }

    while(1) {
    printf("Waiting for data...\n");
    memset(buffer, '\0', sizeof(buffer));
    addrSize = sizeof(clientAdress);
    recvfrom(sockdf, buffer, SIZE, 0, (struct sockaddr*)&clientAdress, &addrSize); //Data från klient
    currentTime = time(NULL)+ DELTA; //Adderad med sekunder mellan 1900->1970 med idag
    uint32_t network_time = htonl(currentTime);  //Omvandlar host time till network time
    printf("Tagit emot data!\n");
    sendto(sockdf, (char *)&network_time, sizeof(network_time), 0, (struct sockaddr*)&clientAdress, sizeof(serverAdress)); //Data till klient
    printf("Data skickat tillbaka!\n");
    }

    return 0;
}