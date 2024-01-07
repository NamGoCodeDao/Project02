#include "syscall.h"
int main(int argc, char** argv)
{
    char filename[255]; // filename
    int OnlyReadType=0;  //type open file

    //Read name file from console
    int a = Write("Nhap vao ten file: ", 100, consoleOutputID);
    int b = Read(filename, 255, consoleInputID);
    int result =Remove(filename);
    if(result == 0)
    {
        Write("Xoa file thanh cong\n", 100, consoleOutputID);
    }
    else
    {
        Write("Xoa file that bai\n", 100, consoleOutputID);
    }

    Halt();
    return 0;
}
