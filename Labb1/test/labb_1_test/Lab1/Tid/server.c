#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
#define DELTA_TIME 2208988800 //Tiden mellan 1900 och 1970 i sekunder
#define LOCAL_HOST_IP "127.0.0.1"

int main(int argc, char **argv) {

    //Vid exekvering välj port
    if(argc != 2) {
        printf("Format: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    time_t currentTime;

    int serverSocket;
    struct sockaddr_in serverAdress, clientAdress;
    char buffer[BUFFER_SIZE];
    socklen_t addrSize; //Används för att allokera minne till adresstrukturen

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0); //Skapar en UDP socket/fildeskriptor
    if(serverSocket < 0) {
        perror("Socket error");
        exit(1);
    }

    memset(&serverAdress, '\0', sizeof(serverAdress)); //Nullar addresstrukturen
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(port);
    serverAdress.sin_addr.s_addr = inet_addr(LOCAL_HOST_IP);

    if(bind(serverSocket, (struct sockaddr*)&serverAdress, sizeof(serverAdress)) < 0) {
        perror("Bind error");
        exit(1);
    }

    while(1) {
    printf("Waiting for data...\n");
    memset(buffer, '\0', sizeof(buffer)); //Nullar buffer
    addrSize = sizeof(clientAdress);
    recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAdress, &addrSize); //Tar emot paket från klient
    currentTime = time(NULL)+ DELTA_TIME; //Tid i sekunder från 1970 till idag, adderad med sekunder mellan 1900->1970
    uint32_t network_time = htonl(currentTime);  //Gör om från "host time" till "network time"
    printf("Data mottaget!\n");
    sendto(serverSocket, (char *)&network_time, sizeof(network_time), 0, (struct sockaddr*)&clientAdress, sizeof(serverAdress)); //Skickar tillbaka data till klient
    printf("Data skickat tillbaka!\n");
    }

    return 0;
}