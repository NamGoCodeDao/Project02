#include "syscall.h"


int main()
{
    char used_item;
    int i = 0;
    int file_id;
    char item[100];
    int k=0;
    int num_new=0;
    while (1)
    {

        //Lock quyen truy cap vao items
        Wait("accessItem");

        //Mo file items
        file_id= Open("items.txt", OnlyRead);

        Read(item, 100, file_id);

        //Lay 1 item ra khoi items
        used_item = item[0];
        Write("Consume:",8,consoleOutputID);
        Write(&used_item, 1, consoleOutputID);
        i=0;
        while (item[i] != 'e')
        {
            item[i] = item[i + 1];
            i++;
        }
        num_new = i;
        if (num_new==1)
        {
            Wait("empty");
        }
        else
        {
            Close(file_id);
            Remove("items.txt");
            Create("items.txt");
            file_id = Open("items.txt", ReadAndWrite);
            Write(item, num_new, file_id);
            Close(file_id);
        }
        //Unlock quyen truy cap vao items
        Signal("accessItem");
    }
}