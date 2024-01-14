#include "syscall.h"
/*
    Ham main cua producer
    Chi tao ra toi da 10 item
*/

int main()
{
    int MAX_ITEMS_PRODUCE = 20;  //So item tieu thu toi da
    int file_id;   //File id cua file items.txt
    int num_item = 0;  //So item hien tai trong items.txt
    char c;     //Bien doc file
    char list_items[100];   //Danh sach item hien tai
    char new_item;  //Item moi duoc tao ra
    int num_item_produced = 0;  //So item da duoc tao ra
    int last_item;  //Item cuoi cung trong danh sach item hien tai
    while(1)
    {

        //Kiem tra so luong item da duoc tao ra
        if (num_item_produced>= MAX_ITEMS_PRODUCE)
        {
            break;
        }
        //Wait empty
        Wait("full");
        //Lock file items.txt
        Wait("accessItem");

        //Open file items.txt
        file_id = Open("items.txt",ReadAndWrite);

        //Count number of current items
        Seek(0, file_id);
        num_item = 0;
        while (Read(&c, 1, file_id) > 0)
        {
            if (c != 'e' && c >= '0' && c <= '9'){
                list_items[num_item] = c;
                num_item++;
            }
        }

        //Neu chua co item nao thi tao item dau tien
        if (num_item == 0)
        {
            list_items[num_item++] = '0';
            list_items[num_item++] = 'e';
            new_item = '0';
        }
        else
        {
            //Lay thong tin item cuoi cung
            last_item=list_items[num_item-1];

            //Tao item moi
            new_item = last_item+1;
            if (new_item > '9')
                new_item = '0';
            list_items[num_item++] = new_item;
            list_items[num_item++] = 'e';
        }

        //In ra man hinh item vua tao
        Write("Produce:",8,consoleOutputID);
        Write(&new_item,1,consoleOutputID);
        Write("\n",1,consoleOutputID);

        //Cap nhat danh sach item vao file
        Close(file_id);
        Remove("items.txt");
        Create("items.txt");
        file_id = Open("items.txt",ReadAndWrite);

        Write(list_items, num_item, file_id);
        num_item_produced++;

        //Close file items.txt
        Close(file_id);
        //Unlock file items.txt
        Signal("accessItem");
        Signal("empty");
    }
    Halt();
    return 0;
}
