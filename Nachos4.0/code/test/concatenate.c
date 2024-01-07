#include "syscall.h"
int main()
{
    int file1ID, file2ID;
    char filename1[32]; //Read data from file 1
    char filename2[32]; //Read data from file 2
    char buffer[128]; //Read data from file 2
    int size = 0; //num character read from file 2
    Write("Input file name 1: ", 100, consoleOutputID);
    Read(filename1, 255, consoleInputID);
    file1ID = Open(filename1, ReadAndWrite);
    if (file1ID != -1)
    {
        Write("Input file name 2: ", 100, consoleOutputID);
        Read(filename2, 255, consoleInputID);
        file2ID = Open(filename2,OnlyRead);
        if (file2ID != -1)
        {
            size = Read(buffer, 255, file2ID);
            Seek(-1, file1ID);
            Write(buffer, size, file1ID);
            Close(file2ID);
            Close(file1ID);
            Write("Concatenate 2 file successfully\n", 100, consoleOutputID);
        }
        else
        {
            Write("Can't Open File 2\n", 100, consoleOutputID);
        }
    }
    else
    {
        Write("Can't Open File 1\n", 100, consoleOutputID);
    }
    Halt();
    return 0;
}
