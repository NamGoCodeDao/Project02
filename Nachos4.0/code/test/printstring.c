#include "syscall.h"

int main() {
    Write("Print string is starting\n", 100, consoleOutputID);
    Write("Print string is exiting\n", 100, consoleOutputID);
    Exit(0);
}
