#include "syscall.h"


int main(){
    char filename1[32]; // filename source
    int newProc;

    while(1){
        Write("Nhap vao ten chuong trinh: ", 100, consoleOutputID);
        Read(filename1, 255, consoleInputID);
        if (filename1[0]=='e' && filename1[1]=='x' &&
            filename1[2]=='i' && filename1[3]=='t')
            Halt();
        newProc = Exec(filename1);
        Join(newProc);
    }
    Halt();
}