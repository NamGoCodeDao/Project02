#include "ptable.h"
#include "bitmap.h"
#include "synch.h"

PTable::PTable(int size)
{
    DEBUG(dbgThread, "Initializing Process Table...");
    psize = size;
    bm = new Bitmap(psize);
    bmsem = new Semaphore("bmsem", 1);

    for (int i = 0; i < MAX_PROCESS; ++i)
    {
        pcb[i] = NULL;
    }
}

PTable::~PTable()
{
    delete bm;
    delete bmsem;
    for (int i = 0; i < psize; ++i)
    {
        if (pcb[i] != NULL)
        {
            delete pcb[i];
        }
    }
}

int PTable::GetFreeSlot()
{
    return bm->FindAndSet();
}

bool PTable::IsExist(int pid)
{
    if (pid < 0 || pid >= MAX_PROCESS)
    {
        return FALSE;
    }

    return pcb[pid] != NULL;
}

void PTable::Remove(int pid)
{
    if (IsExist(pid))
    {
        bm->Clear(pid);
        delete pcb[pid];
        pcb[pid] = NULL;
        psize--;
    }
}

void PTable::LoadMainThread(char *name, Thread *thread)
{
    pcb[0] = new PCB(name, thread);
    psize = 1;
    bm->Mark(0);
}
int PTable::GetCurrentThreadId()
{
    int currentThreadId = -1;
    DEBUG(dbgThread, "PTable: Current thread name " << kernel->currentThread->getName());
    if (strcmp(kernel->currentThread->getName(), "main") == 0)
    {
        currentThreadId = 0;
    }
    else
    {
        for (int i = 1; i < MAX_PROCESS; ++i)
        {
            if (pcb[i] != NULL && 
            strcmp(pcb[i]->GetFileName(), kernel->currentThread->getName()) == 0)
            {
                currentThreadId = i;
                break;
            }
        }
    }
    return currentThreadId;
}

int PTable::ExecUpdate(char *name)
{   
    bmsem->P();

    // check the validity of the program name
    if (name == NULL)
    {
        printf("PTable: Invalid program name\n");
        bmsem->V();
        return -1;
    }

    // Check if the file exists
    OpenFile *executable = kernel->fileSystem->Open(name);
    if (executable == NULL)
    {
        printf("PTable: Unable to open file %s\n", name);
        bmsem->V();
        return -1;
    }
    delete executable;


    int currentThreadId = GetCurrentThreadId();

    // Make sure this program is not called Exec itself.
    if (strcmp(pcb[currentThreadId]->GetFileName(), name) == 0) {
        printf("PTable: Cannot exec self.\n");
        bmsem->V();
        return -1;
    }

    DEBUG(dbgThread, "PTable: ExecUpdate " << name << " from thread with name " << pcb[currentThreadId]->GetFileName());
    

    // Allocate a new PCB
    int freeSlot = GetFreeSlot();
    if (freeSlot == -1)
    {
        printf("PTable: No free slot available.\n");
        bmsem->V();
        return -1;
    }


    pcb[freeSlot] = new PCB(freeSlot);

    // Set parentID
    pcb[freeSlot]->parentID = currentThreadId;

    // Load the executable file
    int result = pcb[freeSlot]->Exec(name, freeSlot);

    if (result == -1)
    {
        printf("PTable: Unable to load executable file %s\n", name);
        delete pcb[freeSlot];
        pcb[freeSlot] = NULL;
        bmsem->V();
        return -1;
    }

    psize++;
    bmsem->V();
    return result;
}

int PTable::JoinUpdate(int id)
{
    int currentThreadId = GetCurrentThreadId();

    if (!IsExist(id))
    {
        printf("PTable: Process with id %d does not exist\n", id);
        return -1;
    }

    if (id == currentThreadId)
    {
        printf("PTable: Cannot join self\n");
        return -1;
    }
    else if (pcb[id]->parentID != currentThreadId)
    {
        printf("PTable: Process with id %d is not a child of current process\n", id);
        return -1;
    }

    pcb[currentThreadId]->IncNumWait();
    pcb[id]->JoinWait();

    pcb[currentThreadId]->DecNumWait();
    pcb[id]->ExitRelease();

    return pcb[id]->GetExitCode();
}

int PTable::ExitUpdate(int exitcode)
{
    int currentThreadId = GetCurrentThreadId();
    if (currentThreadId == 0)
    {
        DEBUG(dbgThread, "PTable: Main thread is exiting");
        kernel->interrupt->Halt();
    }
    else
    {
        pcb[currentThreadId]->SetExitCode(exitcode);
        pcb[currentThreadId]->JoinRelease();
        pcb[currentThreadId]->ExitWait();
        Remove(currentThreadId);
    }
    return exitcode;
}
