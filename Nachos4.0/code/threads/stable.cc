#include "stable.h"

// Constructor
STable::STable()
{
    this->bm = new Bitmap(MAX_SEMAPHORE);

    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        semTab[i] = NULL;
    }
}

// Destructor
STable::~STable()
{
    if (bm != NULL)
    {
        delete bm;
        bm = NULL;
    }
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (semTab[i] != NULL)
        {
            delete semTab[i];
            semTab[i] = NULL;
        }
    }
}

int STable::Create(char *name, int value)
{

    // Check exists semphore
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        if (bm->Test(i))
        {
            if (strcmp(name, semTab[i]->GetName()) == 0)
            {
                DEBUG(dbgSynch, "STable: Find existed semaphore");
                return -1;
            }
        }
    }
    // Find free slot in table
    int freeSlot = bm->FindAndSet();

    if (freeSlot < 0)
    {
        DEBUG(dbgSynch, "STable: Error table is full");
        return -1;
    }

    semTab[freeSlot] = new Sem(name, value);

    return 0;
}

int STable::Wait(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        // Check does slot[i] load semaphore
        if (bm->Test(i))
        {
            if (strcmp(name, semTab[i]->GetName()) == 0)
            {
                semTab[i]->wait();
                return 0;
            }
        }
    }
    DEBUG(dbgSynch, "Not exists semaphore in wait");
    return -1;
}

int STable::Signal(char *name)
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
    {
        // Check does slot[i] load semaphore
        if (bm->Test(i))
        {
            if (strcmp(name, semTab[i]->GetName()) == 0)
            {
                semTab[i]->signal();
                return 0;
            }
        }
    }
    DEBUG(dbgSynch, "Not exists semaphore in signal");
    return -1;
}

int STable::FindFreeSlot()
{
    return bm->FindAndSet();
}