#include "syscall.h"


int main()
{
    const int MAX_ITEM_CONSUME = 10;
    char used_item;
    int i = 0;
    int file_id;
    char item[100];
    int k=0;
    int num_new=0;
    int num_item_consumed=0;
    while (1)
    {

        if (num_item_consumed>=MAX_ITEM_CONSUME)
        {
            break;
        }
        //Lock quyen truy cap vao items
        Wait("empty");
        Wait("accessItem");

        //Mo file items
        file_id= Open("items.txt", OnlyRead);
        Read(item, 100, file_id);

        //Lay 1 item ra khoi items
        used_item = item[0];
        Write("Consume:",8,consoleOutputID);
        Write(&used_item, 1, consoleOutputID);
        Write("\n",1,consoleOutputID);


        //Xoa item vua lay ra khoi items
        i=0;
        while (item[i] != 'e')
        {
            item[i] = item[i + 1];
            i++;
        }
        num_new = i;

        Close(file_id);
        Remove("items.txt");
        Create("items.txt");
        file_id = Open("items.txt", ReadAndWrite);
        Write(item, num_new, file_id);

        //Dong file items
        Close(file_id);
        num_item_consumed++;
        //Unlock quyen truy cap vao items
        Signal("accessItem");
        Signal("full");
    }
    Halt();
    return 0;
}
