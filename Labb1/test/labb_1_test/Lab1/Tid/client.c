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

    int clientSocket;
    struct sockaddr_in serverAdress;
    char buffer[BUFFER_SIZE];
    socklen_t addrSize; //Används för att allokera minne till adresstrukturen

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0); //Skapar en UDP socket/fildeskriptor
    memset(&serverAdress, '\0', sizeof(serverAdress)); //Nullar addresstrukturen
    serverAdress.sin_family = AF_INET;
    serverAdress.sin_port = htons(port);
    serverAdress.sin_addr.s_addr = inet_addr(LOCAL_HOST_IP);

    memset(buffer, '\0', sizeof(buffer)); //Nullar buffert
    sendto(clientSocket, buffer, 0, 0, (struct sockaddr*)&serverAdress, sizeof(serverAdress)); //Skickar tomt UDP packet till server
    printf("Paket skickat!\n");
    recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAdress, &addrSize); //Tar emot data från servern
    currentTime = ntohl(*(unsigned long *) buffer) - DELTA_TIME; //Gör om från "network time" till "host time"
    printf("Data mottaget: %s\n", ctime(&currentTime));


    return 0;
}