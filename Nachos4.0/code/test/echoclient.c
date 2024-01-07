#include "syscall.h"
#include "copyright.h"
#define maxlenmasage 50

int main()
{

    const int numSocket = 4;
    int socketid[numSocket];
    char *messagesend[50];
    char *messagerecv[50];
    int iter = 0;
    // Create 4 socket
    while (iter < numSocket)
    {
        socketid[iter] = SocketTCP();

        if (socketid[iter] != 0)
        {
            Write("Socket created\n", 50, consoleOutputID);
            iter++;
        }
        else
        {
            Write("Socket created failed\n", 50, consoleOutputID);
            numSocket--;
        }
    }

    // Connect each socket to Server with port 8888 and ip "0.0.0.0"
    iter = 0;
    while (iter < numSocket)
    {
        // Connect to server
        int isConnected = Connect(socketid[iter], "127.0.0.1", 1234);
        if(isConnected == -1){
            Write("Connect failed\n", 50, consoleOutputID);
            iter++;
            continue;
        }

        Write("Connect success\n", 50, consoleOutputID);

        // Read message from console to send to server
        Write("Input message of socket: ", maxlenmasage, consoleOutputID);
        Read(messagesend[iter], maxlenmasage, consoleInputID);

        // Send message to server
        Send(socketid[iter], messagesend[iter], maxlenmasage);

        // Receive message from server
        Receive(socketid[iter], messagerecv[iter], maxlenmasage);

        // Print message received from server
        Write("Messages received: ", 50, consoleOutputID);
        Write(messagerecv[iter], maxlenmasage, consoleOutputID);
        Write("\n", 1, consoleOutputID);
        iter++;
    }

    // Close all socket
    iter = 0;
    while (iter < numSocket)
    {
        int isClosed = Close(socketid[iter]);
        if(isClosed == -1){
            Write("Close failed\n", 50, consoleOutputID);
        }
        else{
            Write("Close success\n", 50, consoleOutputID);
        }
        iter++;
    }
    Halt();
    return 0;
}