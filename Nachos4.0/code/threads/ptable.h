
#ifndef PTABLE_H
#define PTABLE_H

#include "pcb.h"
#include "bitmap.h"
#include "synch.h"
#include "thread.h"

#define MAX_PROCESS 10

class PCB;
class Semaphore;

class PTable
{
private:
    int psize;
    Bitmap *bm; // mark the locations that have been used in pcb
    PCB *pcb[MAX_PROCESS];
    Semaphore *bmsem; // used to prevent the case of loading 2 processes at the same time

public:
    // Constructor initialize the size of the PCB object to store the process size. Set the initial value to null
    PTable(int size);
    ~PTable(); // Destructor
    int ExecUpdate(char *);     // Process the syscall SC_Exec
    int ExitUpdate(int);    // Process the syscall SC_Exit
    int JoinUpdate(int);    // Process the syscall SC_Join

    int GetFreeSlot();  // Find the free slot in PTable to save the new process information
    bool IsExist(int pid);    // Check a process exist or not
    void Remove(int pid);    // Delete the PID from the PTable
    char *GetFileName(int id);    // Return the process name

    void LoadMainThread(char *name, Thread* thread); // Load the main process
    int GetCurrentThreadId(); // Return the current thread ID
};

#endif // PTABLE_H