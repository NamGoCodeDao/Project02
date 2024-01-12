#include "syscall.h"


int main(){
    char filename1[32]; // filename source
    int newProc;

    while(1){
        Write("Nhap vao ten chuong trinh: ", 100, consoleOutputID);
        Read(filename1, 255, consoleInputID);
        newProc = Exec(filename1);
        Join(newProc);
    }
    Halt();
}