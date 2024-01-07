#include "syscall.h"
int main(int argc, char **argv)
{
    char filename[255];   // filename
    char buffer[255];     // text read from file and write to console
    int size = 0;         // num character read from file
    int OnlyReadType = 0; // type open file

    // Read name file from console
    int a = Write("Nhap vao ten file: ", 100, consoleOutputID);
    int b = Read(filename, 255, consoleInputID);

    // Open file and read
    int fileID = Open(filename, OnlyReadType);
    if (fileID != -1)
    {
        size = Read(buffer, 255, fileID);
        Write("Noi dung file: \n", 100, consoleOutputID);
        Write("========================= \n", 100, consoleOutputID);
        Write(buffer, size, consoleOutputID);
        Write("\n", 1, consoleOutputID);
        Write("========================= \n", 100, consoleOutputID);
        
        Close(fileID);
    }
    else
    {
        Write("Can't Open File\n", 100, consoleOutputID);
    }
    Halt();
    return 0;
}
