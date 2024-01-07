#include "syscall.h"
int main(int argc, char** argv)
{
    char filename1[32]; // filename source
    char filename2[32]; // filename destination
    int fileID1=-1;      //fileID source
    int fileID2=-1;      //fileID destination
    char buffer[255];  //buffer to copy file
    int size = 0;      //Num of char read from file source

    //Read name file source from console
    Write("Nhap vao ten file nguon: ", 100, consoleOutputID);
    Read(filename1, 255, consoleInputID);
    //Open file source
    fileID1 = Open(filename1, OnlyRead);
    if(fileID1 == -1)
    {
        Write("Khong the mo file nguon\n", 100, consoleOutputID);
    }else
    {
        //Read name file destination from console
        Write("Nhap vao ten file dich: ", 100, consoleOutputID);
        Read(filename2, 255, consoleInputID);

        //Open file destination
        fileID2 = Open(filename2, ReadAndWrite);
        if(fileID2 == -1)
        {
            Write("Khong the mo file dich\n", 100, consoleOutputID);
        }
        else
        {
            //Copy file:
            //<1> Read file from source
            size=Read(buffer,255, fileID1);
            //<2> Write file to destination
            Seek(0,fileID2);
            Write(buffer, size, fileID2);
            //<3> Close file source and destination
            Close(fileID1);
            Close(fileID2);
            Write("Copy file thanh cong\n", 100, consoleOutputID);
        }
    }
    Halt();
    return 0;
}
