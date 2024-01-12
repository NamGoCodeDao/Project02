#include "syscall.h"


int main()
{
    int MAX_ITEMS = 10;
    int i=0;
    int file_id;
    int num_item = 0;
    char c;
    char last_item;
    char list_items[100];
    char new_item;
    while(1)
    {
        //Lock file items.txt
        Wait("accessItem");

        //Open file items.txt
        file_id = Open("items.txt",ReadAndWrite);
        if (file_id == -1)
        {
            Write("Producer: Can not open file items.txt\n", 40, consoleOutputID);
            return 0;
        }

        //Count number of current items
        Seek(0, file_id);
        while (Read(&c, 1, file_id) > 0)
        {
            if (c != '\0' && c >= '0' && c <= '9'){
                list_items[num_item] = c;
                num_item++;
            }
        }

        //Kiem tra so luong item
        if (num_item>= MAX_ITEMS)
        {
            //Thong bao day
            Write("Producer: Buffer is full\n", 25, consoleOutputID);

            //Dong file
            Close(file_id);

            //Unlock file items.txt
            Signal("accessItem");
            return 0;
        }

        //Lay thong tin item cuoi cung
        last_item=list_items[num_item-1];

        //Thong bao san sang
        Write("Producer: Producing item\n", 25, consoleOutputID);

        new_item = last_item+ 1;
        if (new_item > '9')
            new_item = '0';
        
        Seek(-1, file_id);
        Write(&new_item, 1, file_id);

        Close(file_id);
        //Unlock file items.txt
        Signal("accessItem");
        Signal("empty");
    }
    return 0;
}