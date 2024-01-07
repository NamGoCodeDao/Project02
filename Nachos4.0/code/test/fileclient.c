#include "syscall.h"
int main(int argc, char **argv)
{
    char filename1[32]; // filename source
    char filename2[32]; // filename destination
    int fileID1 = -1;   // fileID source
    int fileID2 = -1;   // fileID destination

    int OnlyReadType = 1;  // type open file source
    int ReadWriteType = 0; // type open file destination

    char buffersend[255]; // buffer to send
    char bufferrecv[255]; // buffer to receive

    int readBytes;
    int socketID;
    int isConnected;
    int sendBytes;
    int receiveBytes;

    // Read name file source from console
    Write("Nhap vao ten file nguon: ", 100, consoleOutputID);
    Read(filename1, 255, consoleInputID);

    // Open file source
    fileID1 = Open(filename1, OnlyReadType);
    if (fileID1 == -1)
    {
        Write("Khong the mo file nguon\n", 100, consoleOutputID);
        Halt();
    }

    // Read name file destination from console
    Write("Nhap vao ten file dich: ", 100, consoleOutputID);
    Read(filename2, 255, consoleInputID);

    // Open file destination
    fileID2 = Open(filename2, ReadWriteType);
    if (fileID2 == -1)
    {
        Create(filename2);
    }
    // Open file destination again
    fileID2 = Open(filename2, ReadWriteType);
    if (fileID2 == -1)
    {
        Write("Khong the mo file dich\n", 100, consoleOutputID);
        Halt();
    }

    // Read file from source
    readBytes = Read(buffersend, 255, fileID1);

    // Create socket 
    socketID = SocketTCP();
    if (socketID == -1)
    {
        Write("Khong the tao socket \n", 100, consoleOutputID);
        Halt();
    }

    // Connect to server
    isConnected = Connect(socketID, "127.0.0.1", 1234);
    if (isConnected == -1)
    {
        Write("Khong the ket noi den server \n", 100, consoleOutputID);
        Halt();
    }

    // Send data to server
    sendBytes = Send(socketID, buffersend, readBytes);
    if (sendBytes == -1)
    {
        Write("Khong the gui data den socket \n", 100, consoleOutputID);
        Halt();
    }
    else if (sendBytes == 0)
    {
        Write("Khong the gui data vi socket da dong \n", 100, consoleOutputID);
        Halt();
    }

    // Receive data from server
    receiveBytes = Receive(socketID, bufferrecv, 100);
    if (receiveBytes == -1)
    {
        Write("Khong the nhan data tu socket \n", 100, consoleOutputID);
        Halt();
    }
    else if (receiveBytes == 0)
    {
        Write("Khong the nhan data vi socket da dong \n", 100, consoleOutputID);
        Halt();
    }

    // Write data to destination
    Write(bufferrecv, receiveBytes, fileID2);
    // Close socket
    Close_Socket(socketID);
    // Close file source and destination
    Close(fileID1);
    Close(fileID2);

    Write("Gui nhan du lieu thanh cong\n", 100, consoleOutputID);
    Halt();
    return 0;
}
