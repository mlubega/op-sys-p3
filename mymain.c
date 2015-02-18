/* use first free space for allocation */
#include <assert.h>
#include <stdlib.h>
#include "mem.h"
#include <stdio.h>


int main() {
   assert(Mem_Init(4096) == 0);
   void * ptr[6];
   ptr[0] = Mem_Alloc(400, FIRSTFIT);
   printf("ptr[0] is : %p\n", ptr[0]);
//   assert(ptr[0] != NULL);
   Mem_Dump();
   ptr[1] = Mem_Alloc(400, FIRSTFIT);
   printf("ptr[1] is : %p\n",ptr[1]);
 //  assert(ptr[1] != NULL);

   Mem_Dump();
   ptr[2] = Mem_Alloc(1000, FIRSTFIT);
   printf("ptr[2] is : %p\n", ptr[2]);
 //  assert(ptr[2] != NULL);

   Mem_Dump();
   ptr[3] = Mem_Alloc(1000, FIRSTFIT);
   printf("ptr[3] is : %p\n", ptr[3]);
  // assert(ptr[3] != NULL);

   Mem_Dump();
   ptr[4] = Mem_Alloc(400, FIRSTFIT);
   printf("ptr[4] is : %p\n", ptr[4]);
   //assert(ptr[4] != NULL);

   Mem_Dump();
   ptr[5] = Mem_Alloc(400, FIRSTFIT);
   printf("ptr[5] is : %p\n",  ptr[5]);
 //  assert(ptr[5] != NULL);

   Mem_Dump();
   // use up rest of memory
   void * mem[100];
   int i = 0;

   mem[0]= Mem_Alloc(360, FIRSTFIT);
   printf(" mem[%d] is: %p\n",i,  mem[0]);
   Mem_Dump();
  
   while(mem[i] != NULL){
	i++;
	mem[i] = Mem_Alloc(360, FIRSTFIT);
   Mem_Dump();
        printf(" mem[%d] is:%p \n",i,  mem[0]);
    }


   printf("Mem_Free(ptr[0] : %d\n", Mem_Free(ptr[0]));
   Mem_Dump();
//   assert(Mem_Free(ptr[0]) == 0);
   ptr[0] = NULL;

   printf("Mem_Free(ptr[2] : %d\n", Mem_Free(ptr[2]));
   Mem_Dump();
//   assert(Mem_Free(ptr[2]) == 0);
   ptr[2] = NULL;

   printf("Mem_Free(ptr[4] : %d\n", Mem_Free(ptr[4]));
   Mem_Dump();
//   assert(Mem_Free(ptr[4]) == 0);
   ptr[4] = NULL;

   ptr[0] = Mem_Alloc(360, FIRSTFIT);
   printf("ptr[0] is : %p\n",ptr[0]);
   Mem_Dump();
 //  assert(ptr[0] != NULL);

   ptr[2] = Mem_Alloc(960, FIRSTFIT);
   printf("ptr[2] is : %p\n", ptr[2]);
   Mem_Dump();
//   assert(ptr[2] != NULL);

   ptr[4] = Mem_Alloc(360, FIRSTFIT);
   printf("ptr[4] is : %p\n", ptr[4]);
   Mem_Dump();
 //  assert(ptr[4] != NULL);

   printf("Mem_Free(ptr[0] : %d\n", Mem_Free(ptr[0]));
   Mem_Dump();
 //  assert(Mem_Free(ptr[0]) == 0);
   ptr[0] = NULL;

   printf("Mem_Free(ptr[2] : %d\n", Mem_Free(ptr[2]));
   Mem_Dump();
 //  assert(Mem_Free(ptr[2]) == 0);
   ptr[2] = NULL;

   printf("Mem_Free(ptr[4] : %d\n", Mem_Free(ptr[4]));
   Mem_Dump();
//   assert(Mem_Free(ptr[4]) == 0);
   ptr[4] = NULL;

   ptr[0] = Mem_Alloc(360, FIRSTFIT);
   printf("ptr[0] is : %p\n",ptr[0]);
   Mem_Dump();
   //assert(ptr[0] != NULL);

   ptr[2] = Mem_Alloc(360, FIRSTFIT);
   printf("ptr[2] is : %p\n",ptr[2]);
   Mem_Dump();
//   assert(ptr[2] != NULL);

   ptr[4] = Mem_Alloc(960, FIRSTFIT);
   printf("ptr[4] is : %p\n",ptr[4]);
   Mem_Dump();
  // assert(ptr[4] == NULL);
   assert(m_error == E_NO_SPACE);

   exit(0);
}
