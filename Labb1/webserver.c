#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 4096
#define DOCUMENT_ROOT "sample_website/"

//Den här funktionen hanterar inkommande förfrågor
void handle_request(int c_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);     /*fyller buffern med nollor*/

    //Läser förfrågan från client
    int bytes_read = read(c_socket, buffer, BUFFER_SIZE);

    //Hämtar väg från förfrågan
    char path[BUFFER_SIZE];
    sscanf(buffer, "GET %s HTTP/1.1", path);

    //Öppnar filen för att läsa
    char file_path[BUFFER_SIZE];
    snprintf(file_path, sizeof(file_path), "%s%s", DOCUMENT_ROOT, path);
    FILE* file = fopen(file_path, "r");

    if (file == NULL) {
        //Retunerar 404 error om filen ej hittas
        char response[] = "HTTP/1.1 404 Not Found\r\n\r\n";
        write(c_socket, response, strlen(response));
    } else {
        //Retunerar innehållet av filen till clienten
        char response[] = "HTTP/1.1 200 OK\r\n\r\n";
        write(c_socket, response, strlen(response));

        while (1) {
            int bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
            if (bytes_read <= 0) {
                break;
            }
            write(c_socket, buffer, bytes_read);
        }

        fclose(file);
    }

    //Stänger socketen
    close(c_socket);
}

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;

    //När programet körs kan man välja ett annat portnummer
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    //Skapar en socket för servern
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);    /*skapar en ny komunikations slutpunkt*/

    //Binder socketen till porten
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    //Börja lyssna för inkommande uppkopplingar
    listen(server_socket, 5);

    printf("Server listening on port %d...\n", port);

    while (1) {
        //Acepterar en uppkoppling från en client
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int c_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);

        printf("Incoming connection from %s:%d...\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        //Hanterar förfrågan från client
        handle_request(c_socket);
    }

    //Dödar socket
    close(server_socket);

    return 0;
}