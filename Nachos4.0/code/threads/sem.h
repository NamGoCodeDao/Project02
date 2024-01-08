#ifndef SEM_H
#define SEM_H
#include "synch.h"
class Semaphore;

class Sem
{
private:
    char name[50];  // The semaphore name
    Semaphore *sem; // Create semaphore for management
public:
    // Initial the Sem object, the started value is null
    // Remember to initial the Sem to use
    Sem(char *na, int i);
    ~Sem();
    void wait();
    void signal();
    char *GetName(); // Return the semaphore name
};

#endif // SEM_H