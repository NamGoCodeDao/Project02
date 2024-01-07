#include "syscall.h"
int main()
{
    int a = Write("Nhap vao ten file hoac de trong de dung ten file mac dinh (text.txt): \n", 100, consoleOutputID);
    char filename[255];
    int b = Read(filename, 255, consoleInputID);

    if (filename[0] == 0)
    {
                int c = Create("text.txt");
        if (c != -1)
        {
            Write("Tao file thanh cong\n", 100, consoleOutputID);
        }
        else
        {
            Write("Tao file khong thanh cong\n", 100, consoleOutputID);
        }
    }

    else
    {
        int c = Create(filename);
        if (c != -1)
        {
            Write("Tao file thanh cong\n", 100, consoleOutputID);
        }
        else
        {
            Write("Tao file khong thanh cong\n", 100, consoleOutputID);
        }
    }
    Halt();
    return 0;
}