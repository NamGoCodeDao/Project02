#include "ptable.h"
#include "bitmap.h"
#include "synch.h"

PTable::PTable(int size)
{
    DEBUG(dbgSys, "Initializing Process Table...");
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
    DEBUG(dbgSys, "PTable: Current thread name " << kernel->currentThread->getName());
    if (strcmp(kernel->currentThread->getName(), "main") == 0)
    {
        currentThreadId = 0;
    }
    else
    {
        for (int i = 1; i < MAX_PROCESS; ++i)
        {
            if (pcb[i] != NULL && strcmp(pcb[i]->GetFileName(), kernel->currentThread->getName()) == 0)
            {
                currentThreadId = i;
                break;
            }
        }
    }
    return currentThreadId;
}

int PTable::ExecUpdate(char *fileName)
{   
    DEBUG(dbgSys, "PTable: ExecUpdate " << fileName);
    bmsem->P();

    // // Prevent self-execution
    int currentThreadId = GetCurrentThreadId();
    DEBUG(dbgSys, "PTable: Current thread id " << currentThreadId);

    if (strcmp(pcb[currentThreadId]->GetFileName(), fileName) == 0) {
        fprintf(stderr, "PTable: Cannot exec self.\n");
        bmsem->V();
        return -1;
    }
    DEBUG(dbgSys, "PTable: ExecUpdate " << fileName << " from thread with name " << pcb[currentThreadId]->GetFileName());
    

    // Allocate a new PCB
    int freeSlot = GetFreeSlot();
    if (freeSlot == -1)
    {
        fprintf(stderr, "PTable: No free slot available.\n");
        bmsem->V();
        return -1;
    }

    DEBUG(dbgSys, "PTable: Free slot " << freeSlot << " allocated.");

    pcb[freeSlot] = new PCB(freeSlot);

    // Set parentID
    pcb[freeSlot]->parentID = currentThreadId;

    // Load the executable file
    int result = pcb[freeSlot]->Exec(fileName, freeSlot);
    psize++;
    bmsem->V();
    return result;
}

int PTable::JoinUpdate(int id)
{
    int currentThreadId = GetCurrentThreadId();

    if (!IsExist(id))
    {
        fprintf(stderr, "PTable: Process with id %d does not exist\n", id);
        return -1;
    }

    if (id == currentThreadId)
    {
        fprintf(stderr, "PTable: Cannot join self\n");
        return -1;
    }
    else if (pcb[id]->parentID != currentThreadId)
    {
        fprintf(stderr, "PTable: Process with id %d is not a child of current process\n", id);
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
        kernel->interrupt->Halt();
    }
    else
    {
        pcb[currentThreadId]->SetExitCode(exitcode);
        pcb[currentThreadId]->JoinRelease();
        pcb[currentThreadId]->ExitWait();
        Remove(currentThreadId);
    }
}
