#include "syscall.h"

int main() { 
    int newProc1;
    int newProc2;   
    CreateSemaphore("semaphore1", 1);
    CreateSemaphore("semaphore2", 1);

    Wait("semaphore2");
    newProc2 = Exec("copy"); // Project 01
    Signal("semaphore2");
    
    Wait("semaphore1");
    newProc1 = Exec("cat"); // Project 01
    Signal("semaphore1");

    Join(newProc1);
    Join(newProc2);
    Halt();
}