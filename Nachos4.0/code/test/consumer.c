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
        i=0;
        item[0] = '\0';
        //Dam bao chi su dung item khi co trong items
        Wait("empty");

        //Lock quyen truy cap vao items
        Wait("accessItems");

        //Mo file items
        file_id= Open("items.txt", OnlyRead);

        Read(item, 100, file_id);

        //Lay 1 item ra khoi items
        used_item = item[0];

        //Thong bao da lay item ra khoi items
        //Write("Consumer: Using item \n", 22, consoleOutputID);
        //Write(&used_item, 1, consoleOutputID);
        //Xoa item vua lay ra khoi items
        Write(&used_item, 1, consoleOutputID);
        while (item[i+1] != '\0')
        {
            item[i] = item[i + 1]; 
            i++;
        }
        num_new = i-1;


        Close(file_id);
        Remove("items.txt");
        Create("items.txt");
        file_id = Open("items.txt", ReadAndWrite);
        Write(item, num_new, file_id);
        Close(file_id);


        //Unlock quyen truy cap vao items
        Signal("accessItems");
    }
}