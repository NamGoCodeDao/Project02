#ifndef PCB_H
#define PCB_H

#include "synch.h"
#include "thread.h"

class Semaphore;
// The process control block
//
// This class stores and manages information necessary to execute and join
// a process, including:
// - Process ID
// - Parent ID
// - The executable file containing the program
// - The thread used to execute the program
// - Information for managing joining operation
//
// Instances of this class are entries in the system-wide process table,
// manage by PTable
class PCB
{
private:
    Semaphore *joinsem; // semaphore for join process
    Semaphore *exitsem; // semaphore for exit process
    Semaphore *multex;  // exclusive access semaphore
    int exitcode;
    int numwait; // the number of join process

    Thread *_thread; // The thread object to be run
    char *_file;     // Name of the executable file to load and run
    int _pid;

public:
    int parentID; // The parent process’s ID

    // PCB(int id);  // Constructor
    // ~PCB(); // Destructor

    // // Initialize the first PCB
    // //
    // // This constructor should be used exclusively for the first program
    // // executed by Nachos. Because we are now supporting process control
    // // blocks for Nachos, so it's obvious that the first process should be the
    // // first one to be allocated to the process table.

    // // Load the program has the name is “filename” and the process id is pid
    // PCB(const char *fileName, Thread *thread);

    // int Exec(char *name, int pid); // Create a thread with the name is filename and the process id is pid;
    // int GetID(); // Return the PID of the current process
    // int GetNumWait(); // Return the number of the waiting process
    // void JoinWait(); // The parent process wait for the child process finishes 
    // void ExitWait(); // The child process finishes
    // void JoinRelease(); // The child process notice the parent process  
    // void ExitRelease(); // The parent process accept to exit the child process 
    // void IncNumWait(); // Increase the number of the waiting process
    // void DecNumWait(); // Decrease the number of the waiting process
    // void SetExitCode(int); // Set the exit code for the process
    // int GetExitCode(); // Return the exitcode
    // void SetFileName(char *); // Set the process name
    // char *GetFileName(); // Return the process name
};

#endif // PCB_H