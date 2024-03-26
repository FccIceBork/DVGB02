#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFFER_SIZE 4096
#define FOLDER_PATH "sample_website/"

int main(int argc, char const *argv[])
{
    int serverSocket, newSocket; // deklarerar fildeskriptorer för sockets
    struct sockaddr_in address; // struct som innehåller IP och port till servern
    int addresslen = sizeof(address); // adresstruktens längd
    char buffer[BUFFER_SIZE] = {0}; // innehåller "data från sockets"

    // Skapa fildeskriptor för socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(1);
    }

    // Ser till så att samma socketadress och portnr kan återanvändas vid omstart av server
    int option = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))) //KOLLA UPPPPPPPPPPPPPPP
    {
        perror("setsockopt failed");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binder socket till serveradress och port
    if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(1);
    }

    // Lyssnar efter anslutningar
    if (listen(serverSocket, 3) < 0)
    {
        perror("listen error");
        exit(1);
    }

    while (1)
    {

        // Godkänner anslutning från klient
        struct sockaddr_in clientAddress;
        socklen_t clientAddress_len = sizeof(clientAddress);
        newSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddress_len);

        // Hanterar begäran från klient
        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', BUFFER_SIZE);
        read(newSocket, buffer, BUFFER_SIZE); //Läser HTTP-request från klient och lägger i "buffer"

        // Hämtar "path" från begäran
        char path[BUFFER_SIZE];
        sscanf(buffer, "GET %s HTTP/1.1", path); //Skannar buffer efter "GET ..." och lägger in i "path"

        char filePath[strlen(path) + strlen(FOLDER_PATH) + 1]; //Array med tillräcklig storlek för hela sökvägen "sample_website/...."
        snprintf(filePath, sizeof(filePath), "%s%s", FOLDER_PATH, path); //"Slår ihop" mappsökvägen med "path"
        FILE *file = fopen(filePath, "r"); // Öppnar fil för läsning

        if (file == NULL)
        {
            // Ger 404 vid fel
            char response[] = "HTTP/1.1 404 Not Found\r\n\r\n";
            write(newSocket, response, strlen(response));
        }
        else
        {
            // Returnerar filinnehåll till klient
            char response[] = "HTTP/1.1 200 OK\r\n\r\n";
            write(newSocket, response, strlen(response));

            while (1)
            {
                int bytesRead = fread(buffer, 1, BUFFER_SIZE, file);
                if (bytesRead <= 0)
                {
                    break;
                }
                write(newSocket, buffer, bytesRead);
            }

            fclose(file);
        }

        // Avslutar klientsocket
        close(newSocket);

    }
    return 0;
}