#include "syscall.h"
int main()
{
   int const MAX_ITEMS = 10;
  SpaceId consumerID, producerID;
   //Create semaphore to control the access to the items
   CreateSemaphore("accessItem", 1);



   //Create semaphore to control empty items
    CreateSemaphore("empty", MAX_ITEMS);




   //Create producer and consumer
   consumerID = Exec("consumer");
   Write("Exec consumer\n", 14, consoleOutputID);

   producerID = Exec("producer");
   Write("Exec producer\n", 14, consoleOutputID);
   //Join producer and consumer
   Join(consumerID);
   Join(producerID);


    return 0;
}