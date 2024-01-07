/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "FileDescriptors.h"
#include "synchconsole.h"

Table table;

void SysHalt()
{
  kernel->interrupt->Halt();
}

char *User2System(int virtAddr, int limit)
{
  int i; // index
  int oneChar;
  char *kernelBuf = NULL;

  kernelBuf = new char[limit + 1]; // need for terminal string
  if (kernelBuf == NULL)
    return kernelBuf;

  memset(kernelBuf, 0, limit + 1);

  // printf("\n Filename u2s:");
  for (i = 0; i < limit; i++)
  {
    kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
    kernelBuf[i] = (char)oneChar;
    // printf("%c",kernelBuf[i]);
    if (oneChar == 0)
      break;
  }
  return kernelBuf;
}

int System2User(int virtAddr, int len, char *buffer)
{
  if (len < 0)
    return -1;
  if (len == 0)
    return len;

  int i = 0;
  int oneChar = 0;

  do
  {
    oneChar = (int)buffer[i];
    kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
    i++;
  } while (i < len && oneChar != 0);

  return i;
}

void IncreasePC()
{
  // set previous programm counter (debugging only)
  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

  // set programm counter to next instruction (all Instructions are 4 byte wide)
  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(NextPCReg));

  // set next programm counter for brach execution
  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

int SysCreate(int virtAddr)
{
  DEBUG(dbgSys, "\n Reading virtual address of filename");
  char *filename;

  DEBUG(dbgSys, "\n Reading filename.");
  // MaxFileLength is 32
  int result = 0;
  filename = User2System(virtAddr, MaxFileLength + 1);

  if (filename == NULL || strlen(filename) == 0)
  {
    DEBUG(dbgSys, "\n Filename is not valid");
    result = -1;
  }
  else
  {
    DEBUG(dbgSys, "\n Finish reading filename.");
    DEBUG(dbgSys, "\n File name : '" << filename << "'");

    // Create file with size = 0
    if (!kernel->fileSystem->Create(filename))
    {
      DEBUG(dbgSys, "\n Error create file '" << filename << "'");
      result = -1;
    }
    else
    {
      DEBUG(dbgSys, "\n Create file '" << filename << "' successfully");
    }
  }

  delete[] filename;
  return result;
}

int SysSeek(int pos, OpenFileId id)
{
  if (id < 0)
  {
    DEBUG(dbgSys, "\n Invalid file descriptor");
    return -1;
  }
  else if(id==consoleOutputID){
    DEBUG(dbgSys, "\n Error: Can not seek console output");
    return -1;
  }
  else if(id==consoleInputID){
    DEBUG(dbgSys, "\n Error: Can not seek console input");
    return -1;
  }
  else if(!table.IsOpening(id))
  {
    DEBUG(dbgSys, "\n Error: File is not opening");
    return -1;
  }
    DEBUG(dbgSys, "\n Seek file");
    return table.GetFileDescriptor(id)->Seek(pos);
}

 int SysOpen(int virtAddr, int type)
 {
   OpenFileID FileID;
   if (type != 0 && type != 1)
   {
     DEBUG(dbgSys, "\n Invalid type");
     return -1;
   }

   DEBUG(dbgSys, "\n Reading virtual address of filename");
   char *filename = User2System(virtAddr, MaxFileLength);

   if (filename == NULL || strlen(filename) == 0)
   {
     DEBUG(dbgSys, "\n Filename is not valid");
     return -1;
   }

   FileID=table.Open(filename, type);
   if (FileID == -1)
   {
     DEBUG(dbgSys, "\n Error open file '" << filename << "'");
     return -1;
   }
   else
   {
     DEBUG(dbgSys, "\n Open file '" << filename << "' successfully");
     return FileID;
   }
   delete[] filename;
 }
 int SysClose(OpenFileId id)
 {
     if (table.Close(id) == -1)
      {
        DEBUG(dbgSys, "\n Error close file '" << id << "'");
        return -1;
      }
      else
      {
          DEBUG(dbgSys, "\n Close file '" << id << "' successfully");
          return 0;
      }
 }
int SysRemove(int virtAddr)
{
  int result = -1;
  char *filename;
  filename = User2System(virtAddr, MaxFileLength);
  DEBUG(dbgSys, "\n Reading virtual address of filename");
  if (filename == NULL || strlen(filename) == 0)
  {
    DEBUG(dbgSys, "\n Filename is not valid");
    result = -1;
  }
  if (table.IsOpeningWithName(filename) != -1)
  {
    DEBUG(dbgSys, "\n File is opening");
    result = -1;
  }
  else if (TRUE)
  {
    if (!kernel->fileSystem->Remove(filename))
    {
      DEBUG(dbgSys, "\n Error remove file '" << filename << "'");
      result = -1;
    }
    else
    {
      DEBUG(dbgSys, "\n Remove file '" << filename << "' successfully");
      result = 0;
    }
  }
  delete[] filename;
  return result;
}

int SysRead(int virtAddr, int size, OpenFileId id)
{
  // Check valid id
  if (id < 0)
  {
    DEBUG(dbgSys, "\n Invalid file descriptor");
    return -1;
  }
  else if (id == consoleOutputID)
  {
    DEBUG(dbgSys, "\n Error: Can not read from console output");
    return -1;
  }
  else if(!table.IsOpening(id))
  {
    DEBUG(dbgSys, "\n Error: File is not opening");
    return -1;
  }

  // Begin reading
  int result = -1;
  char *buffer = User2System(virtAddr, size);

  if (id == consoleInputID)
  {
    DEBUG(dbgSys, "\n Reading from console");
    int i = 0;
    while (i < size)
    {
      buffer[i] = kernel->synchConsoleIn->GetChar();

      if (buffer[i] == EOF || buffer[i] == '\n')
      {
        buffer[i] = '\0';
        break;
      }
      i++;
    }

    DEBUG(dbgSys, "\n Finish reading from console with " << i << " characters");
    result = i;
  }
  else
  {
    DEBUG(dbgSys, "\n Reading from file");
    result = table.GetFileDescriptor(id)->Read(buffer, size);
    DEBUG(dbgSys, "\n Finish reading from file with " << result << " characters");
  }

  DEBUG(dbgSys, "\n Write buffer to user space");
  System2User(virtAddr, size, buffer);
  delete[] buffer;
  return result;
}

int SysWrite(int virtAddr, int size, OpenFileId id)
{
  // Check valid id
  if (id < 0)
  {
    DEBUG(dbgSys, "\n Invalid file descriptor");
    return -1;
  }
  else if (id == consoleInputID)
  {
    DEBUG(dbgSys, "\n Error: Can not write to console input");
    return -1;
  }
  else if(!table.IsOpening(id))
  {
    DEBUG(dbgSys, "\n Error: File is not opening");
    return -1;
  }

  // Begin writing
  int result = -1;
  char *buffer = User2System(virtAddr, size);

  if (id == consoleOutputID)
  {
    DEBUG(dbgSys, "\n Writing to console");
    int i = 0;
    while (buffer[i] != '\0' && i < size)
    {
      kernel->synchConsoleOut->PutChar(buffer[i]);
      i++;
    }

    DEBUG(dbgSys, "\n Finish writing to console with " << i << " characters");
    result = i;
  }
  else
  {
    DEBUG(dbgSys, "\n Writing to file");
    result = table.GetFileDescriptor(id)->Write(buffer, size);
    DEBUG(dbgSys, "\n Finish writing to file with " << result << " characters");
  }

  DEBUG(dbgSys, "\n Write buffer to user space");
  System2User(virtAddr, size, buffer);
  delete[] buffer;
  return result;
}
int SysSocketTCP()
{
    int result = -1;
    int id = table.SocketTCP();
    if (id != -1)
    {
        result = id;
    }
    return result;
}
int SysConnect(int socketid, int ipvirtual, int port)
{
    int result = -1;
    char *ip = User2System(ipvirtual, MaxFileLength);
    if (ip==NULL or strlen(ip)==0)
    {
        DEBUG(dbgSys, "\n IP is not valid");
        return result;
    }
    else
    {
        result = table.Connect(socketid, ip, port);
    }
    return result;
}
int SysSend(int socketid, int virtbuffer, int length)
{
    int result = -1;
    char *buffer = User2System(virtbuffer, length);
    if (buffer == NULL)
    {
        DEBUG(dbgSys, "\n Buffer is not valid");
        return result;
    }
    else
    {
        result = table.Send(socketid, buffer, length);
    }
    return result;
}
int SysReceive(int socketid, int virtbuffer, int length)
{
    int result = -1;
    char *buffer = User2System(virtbuffer, length);
    if (buffer == NULL)
    {
        DEBUG(dbgSys, "\n Buffer is not valid");
        return result;
    }
    else
    {
        result = table.Receive(socketid, buffer, length);
        //Send buffer to user space
        System2User(virtbuffer, length, buffer);
    }
    //Delete buffer
    return result;
}
int SysCloseSocket(int socketid)
{
    int result = -1;
    result = table.Close_Socket(socketid);
    return result;
}
#endif /* ! __USERPROG_KSYSCALL_H__ */