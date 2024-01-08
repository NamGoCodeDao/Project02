#include "pcb.h"
#include "addrspace.h"
#include "main.h"
#include "synch.h"
#include "thread.h"
#include <stdio.h>


PCB::PCB(int id)
{
    _pid = id;
    parentID = -1;
    numwait = 0;
    exitcode = 0;
    _file = NULL;
    _thread = NULL;
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("mutex", 1);
}

// Main thread is created by the kernel so we only need to initialize the PCB
PCB::PCB(char* fileName, Thread* thread)
{
    _pid = 0;
    parentID = -1;
    numwait = 0;
    exitcode = 0;
    _file = new char[strlen(fileName)];
    strcpy(_file, fileName);
    _thread = thread;
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("mutex", 1);
}


PCB::~PCB()
{
    delete _file;
    // if (_thread != NULL){
    //     _thread->Finish();
    //     delete _thread;
    // }
    delete joinsem;
    delete exitsem;
    delete mutex;
}

int PCB::GetID()
{
    return _pid;
}



int PCB::GetNumWait()
{
    return numwait;
}

int PCB::GetExitCode()
{
    return exitcode;
}

void PCB::SetExitCode(int exitCode)
{
    exitcode = exitCode;
}

void PCB::IncNumWait()
{
    mutex->P();
    ++numwait;
    mutex->V();
}

void PCB::DecNumWait()
{
    mutex->P();
    --numwait;
    mutex->V();
}

static void RunProcess(void *args)
{
    char *filename = (char*)args;
    AddrSpace *space = new AddrSpace;
    if (space->Load(filename)) {
        space->Execute();
    }
    ASSERTNOTREACHED();
}   

int PCB::Exec(char *fileName, int pid)
{
    mutex->P();
    DEBUG(dbgThread, "PCB: Setting things up for " << fileName << "...");

    _pid = pid;

    _file = new char[strlen(fileName)];
    strcpy(_file, fileName);

    _thread = new Thread(_file);
    _thread->Fork(RunProcess, (void*)_file);
    mutex->V();
    return _pid;
}

void PCB::JoinWait()
{
    joinsem->P();
}

void PCB::ExitWait()
{
    exitsem->P();
}

void PCB::JoinRelease()
{
    joinsem->V();
}

void PCB::ExitRelease()
{
    exitsem->V();
}


void PCB::SetFileName(char * filename){
    if (_file != NULL) delete _file;
    _file = new char[strlen(filename)];
    strcpy(_file, filename);
}
char* PCB::GetFileName(){
    return _file;
}