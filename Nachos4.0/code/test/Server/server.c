#include <stdio.h> // perror, printf
#include <stdlib.h> // exit, atoi
#include <unistd.h> // read, write, close
#include <arpa/inet.h> // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY, socket etc...
#include <string.h> // memset

void Upper(char a[]) {
    int i = 0;
    while (a[i] != '\0') {
        if (a[i] >= 'a' && a[i] <= 'z') {
            a[i] = a[i] - 32;
        }
        i++;
    }
}
int main(int argc, char const *argv[]) {

    int serverFd, clientFd;
    struct sockaddr_in server, client;
    int len;
    int port = 1234;
    char buffer[1024];
    if (argc == 2) {
        port = atoi(argv[1]);
    }
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("Cannot create socket");
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    len = sizeof(server);
    if (bind(serverFd, (struct sockaddr *)&server, len) < 0) {
        perror("Cannot bind sokcet");
        exit(2);
    }
    if (listen(serverFd, 10) < 0) {
        perror("Listen error");
        exit(3);
    }
    while (1) {
        len = sizeof(client);
        printf("waiting for clients\n");
        if ((clientFd = accept(serverFd, (struct sockaddr *)&client, &len)) < 0) {
            perror("accept error");
            exit(4);
        }
        char *client_ip = inet_ntoa(client.sin_addr);
        printf("Accepted new connection from a client %s:%d\n", client_ip, ntohs(client.sin_port));
        memset(buffer, 0, sizeof(buffer));
        int size = read(clientFd, buffer, sizeof(buffer));
        if ( size < 0 ) {
            perror("read error");
            exit(5);
        }
        if (strcmp(buffer, "exit") == 0) {
            printf("Client %s:%d disconnected\n", client_ip, ntohs(client.sin_port));
            printf("Do you want to close the server? (y/n): ");
            char c;
            scanf("%c", &c);
            if (c == 'y') {
                close(serverFd);
                printf("Server closed\n");
                break;
            }
            continue;
            close(clientFd);
        }

        printf("received %s from client\n", buffer);

        Upper(buffer);
        if (write(clientFd, buffer, size) < 0) {
            perror("write error");
            exit(6);
        }
        close(clientFd);
    }
    close(serverFd);
    return 0;
}