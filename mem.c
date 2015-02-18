#include "mem.h"
#include <stdio.h>
#include "stdlib.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define ALIGN_EIGHT 8
#define MAGIC_NUMBER 123



typedef struct __header_t {
	int size;
	int magic;
	void* fix;
} header_t;


typedef struct __list_t{

	int size;
	struct __list_t *next;
	struct __list_t *prev;



}list_t;


list_t *head = NULL;
list_t *freep = NULL;
list_t *tail = NULL;

int Avail_Size;
int m_error; // for error_handling
int page_size;
int total_size;
/*mona
  void Mem_Coalesce(){


  list_t* temp;
  temp = freep;
//      printf("\nEntered Mem_Coalesce\n");
//      printf("freep addr is : %llx\n", (long long unsigned) freep);



while( temp->next != NULL){
if(((char*)temp +temp->size) == (char*)(temp->next)){
temp->next=(temp->next)->next;
temp->size+=(temp->next)->size;
}
temp=temp->next;
}


}//end coalesce
mona */

//Mem_Coalesece increments to next node and merges upwards;
void Mem_Coalesce(){


	list_t* temp;
	temp = freep;
	//	printf("\nEntered Mem_Coalesce\n");
	//	printf("freep addr is : %llx\n", (long long unsigned) freep);

	if(temp != NULL){
		while( temp->next != NULL){
			temp = temp->next;

			if(((char*)(temp->prev) + ((temp->prev)->size)) == (char*)temp){
				(temp->prev)->size += temp->size;

				//merge node to the tail node
				if(temp-> next == NULL){
					tail = temp->prev;
					(temp->prev)->next = NULL;

					break;
				
				}else{
					//general case
					(temp->next)->prev = temp->prev;
					(temp->prev)->next = temp->next;

					temp = temp->prev;
				}
			}//end if

		}//end while
	}//end if
}//end coalesce

//finds a returns a pointer to first fit
list_t* find_firstfit( list_t* ff, int sizeToAlloc){


	list_t *temp;
	temp = freep;

	do{
		if( (temp->size) >= sizeToAlloc){
			ff=temp;
			break;
		}
		temp = temp->next;

	}while(temp != NULL);

	if(ff == NULL){
		m_error = E_NO_SPACE ;
		printf("NOT enough space\n");

	}
	if (ff!=NULL)
		total_size=-sizeToAlloc;
	return ff;
}

//find worst fit
list_t* find_worstfit(list_t* worst_node, int sizeToAlloc){
	list_t* temp;
	temp = freep;
	worst_node = NULL;

	while(temp != NULL){
		if (temp->size >= sizeToAlloc) {
			if	((worst_node==NULL) || ( (worst_node!=NULL) && ((temp->size) > (worst_node->size) ))){
				worst_node = temp;

			}
		}

		temp = temp->next;
	}
	if(worst_node == NULL){
		m_error = E_NO_SPACE;
		printf("NOT enough space\n");

	}
//	if (worst_node!=NULL)
//		total_size=-sizeToAlloc;

	return worst_node;
}

//find best fit
list_t* find_bestfit(list_t* best_node, int sizeToAlloc){

	best_node=NULL;
	list_t * temp;
	temp=freep;
	while(temp!=NULL){

		if (((temp->size) >= sizeToAlloc) && ((best_node == NULL) || ((temp->size) < (best_node->size)))){
			best_node = temp;
		}

		if((best_node!=NULL )&& ( (best_node->size) == sizeToAlloc)){
			break;
		}
		temp = temp->next;
	}
	if(best_node == NULL){
		m_error = E_NO_SPACE;
		printf("NOT enough space\n");

	}
//	  if (best_node!=NULL)
//testing total_size=-sizeToAlloc;

	return  best_node;
}




int Mem_Init(int sizeOfRegion)
{
	//ensures we cannot call Mem Init more than once
	static unsigned int call_count=0;
	call_count++;

	//pagesize should be 4096
	int page_size=getpagesize();

	//error checking
	if ((sizeOfRegion <= 0) || (head!= NULL) || (call_count>1)  )
	{
		m_error=E_BAD_ARGS;
		printf(" m_error is :%d \n",m_error);
		exit(0);
		//return -1;
	}

	
	// sizeOfRegion (in bytes) needs to be
	// evenly divisible by the page size
	if(sizeOfRegion % page_size != 0) 
	{
		sizeOfRegion += ( page_size - (sizeOfRegion % page_size) );
	}
	total_size=sizeOfRegion;


//test	Avail_Size=sizeOfRegion;

	// open the /dev/zero device
	int fd = open("/dev/zero", O_RDWR);
	void *ptr = mmap(NULL, sizeOfRegion, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{	
		m_error=E_BAD_ARGS; 
		perror("mmap");
		exit(1); 
	}

	close(fd);




	//initialize free list

	freep = tail = (list_t*) ptr;
	freep->next = NULL;
	freep->prev = NULL;
	freep->size = sizeOfRegion;

	//	freep->size = sizeOfRegion - sizeof(list_t);
// test    total_size=sizeOfRegion;
	
	
	//print init data
	printf("End of Mem_Init:\n\n");
	printf("freep->size is %d \n",freep->size);
	printf("sizeof list_t ->size is %zu \n",sizeof(list_t));
	printf("sizeof header_t ->size is %zu \n\n",sizeof(header_t));


	return 0;

}
void *Mem_Alloc(int size, int style){

	//size error check
	if( size <= 0){

		m_error = E_BAD_ARGS;
		printf("Bad size args\n");
		return NULL;
	}

	printf("call to Mem_Alloc(%d)\n", size);     
	
	//8bit align size

	if(size % ALIGN_EIGHT != 0) {
		size += ( ALIGN_EIGHT - (size % ALIGN_EIGHT) );
	}

//test //	printf("sizeOfRegion is %d \n", Avail_Size);
//test //	printf("size: %d\n", size);


	//add header size to 8 bit aligned size, header is 
	//also 8 bit aligned
	int adjusted_size;
	adjusted_size = size + sizeof(header_t);
	
	

//test	//  	printf("size: %d\n", size);
//test	//	printf("adjusted size: %d\n", adjusted_size);
//test	//	printf("freep->size: %d\n", freep->size);

	//error check if there is no free list 
	if(freep->next == NULL &&  (adjusted_size > (freep->size - sizeof(list_t))  ))

	{

//test		//  printf("here\n");
		m_error = E_NO_SPACE;
		return NULL;
	}

	//local variables
	
	list_t * newfreesp;
	//will return this as a ptr to the allocated spac
	list_t *temp = NULL;
       	//declare pointer for the header space
	header_t *headptr;


	//allocate new header
	header_t fillhead;
	fillhead.size = adjusted_size;
	fillhead.magic = 123;

	switch (style){

		case BESTFIT:
			temp = find_bestfit(temp,adjusted_size);
			break;

		case WORSTFIT:
			temp = find_worstfit(temp,adjusted_size);
			break;
		case FIRSTFIT:
			temp = find_firstfit(temp,adjusted_size);
			break;
		default:
			m_error = E_BAD_ARGS; 
			printf("bad style args\n");
			return NULL;           
			break;


	}
//test	//	printf("tmp is : %llx\n", (long long unsigned) temp);
//test	//	printf("free is : %llx\n", (long long unsigned) freep);

	//check if there is a free list but if the size requested leaves
	//less that sizeof(header_t), do not allocate to avoid a seg fault
	if(temp == freep && (freep->next == NULL))
		if (temp->size - adjusted_size < sizeof(list_t))
		{
			m_error=E_NO_SPACE;
			printf("NOT enough space\n");

			temp=NULL;
		}


	//valid free node found
	if(temp != NULL){

		////////////////only one node in free list////////////////////////
		if(temp == freep && (freep->next == NULL)){

			//all of memory requested
			if( adjusted_size == freep->size){
				freep = tail = NULL;
			}
			//only portion requested
			else{  
				//	freep = (list_t*)((char*) temp + sizeof(header_t)+sizeof(list_t)); //not understanding! confused!!!
				//  freep->size = temp->size - adjusted_size-sizeof(header_t);
				//freep->size = temp->size - adjusted_size-sizeof(list_t);
				
				freep = (list_t*)((char*) temp + adjusted_size);
				freep->next=NULL;
				freep->prev=NULL;
				tail = freep;
				freep->size = temp->size - adjusted_size;
			}

		}
		////////////////more than one node in free list///////////////////////
		else{
			//size requested is the same size as free node-
			//in an a free list with more than 1
			if(temp->size == adjusted_size){

				//temp is first node in free list
				if(temp == freep && (freep->next != NULL)){
					freep = temp->next;

				}
				//temp some middle node in the list
				else if(temp != tail){			
					(temp->prev)->next = temp->next;
					(temp->next)->prev = temp->prev;

				}
				//temp is the tail			
				else{
					(temp->prev)->next = NULL;
					tail = temp->prev;

				}


			//sizeToAlloc is only a portion of a node in a list 
			//more than one node
			}else{


				newfreesp =(list_t*) ((char*)temp + adjusted_size);
				//fitz		newfreesp->size = temp->size - adijusted_size- sizeof(list_t);

				newfreesp->size = temp->size - adjusted_size;
				//temp is first node in free list w/ more than one node
				if(temp == freep && (freep->next != NULL) ){
					newfreesp->next = temp->next;
					freep = newfreesp;
				}

				//temp some middle node in the list w/ more than one
				else if(temp != tail){	
					newfreesp->prev = temp->prev;
					newfreesp->next = temp->next;	
					(temp->prev)->next=newfreesp;

				}
				//temp is the tail			
				else{
					//local variables		
					newfreesp->prev = temp->prev;
					newfreesp->next = NULL;	
					(temp->prev)->next=newfreesp;
					tail = newfreesp;
				}

			}
		}//endelse for (more than more node in list)





		//move temp ptr past the header (8bytes)
		//so it points to the beginning of the allocated
		//space
		temp = (list_t*)((char*)temp + sizeof(header_t));

		//initialise header pointer address
		headptr = (header_t*)((char*)temp - sizeof(header_t));

		printf("Mem_Alloc header address assigned is : %llx\n", (long long unsigned) headptr);
		//headerpointer: fill this space with the header		
		*headptr = fillhead;
	
//test		//	printf("header is  : %llx\n", (long long unsigned) headptr);
//test		//	printf("magic num is  %d\n", headptr->magic);


		//cast to void and return pointer to allocated space	
		return (void *) temp;


	}

	else{
		return NULL;
	}

}//end Mem_Alloc
int Mem_Free(void *ptr){


	printf("Entered Mem_Free()\n");
	
	// local variables
	list_t* newptr;
	list_t* temp ; 

	//move to top of header
	header_t *hptr =(header_t*) ( (char*)ptr - sizeof(header_t));


	printf("hptr address in Free() is : %llx\n", (long long unsigned)hptr);
	//	printf("414 ptr address in Free() is : %llx\n", (long long unsigned)ptr);

	printf(" Magic num is %d\n",  hptr->magic);
	printf("size of node to free is : %d\n", hptr->size);
	if( hptr->magic == MAGIC_NUMBER){

		//printf("Valid Pointer, begin free process\n");
		
		//save size of header before changing type (data will be lost)
		int size_free;
		size_free=(hptr->size);
		
		//inititalize node to be added to free list
		newptr = (list_t*) hptr;
		newptr->size=size_free;
		
		//save address variable for comparison purposese
		char*addrToFree = (char*) newptr;

		//free list is empty
		if(freep == NULL){
			freep = newptr;
		}
		//non empty free list
		else{

			temp = freep;

			//addr before start of free list
			if(addrToFree < (char*)temp){

				newptr->prev = NULL;
				temp->prev = newptr;
				newptr->next = temp;
				freep = newptr;

			}
			//addr after the end of free list
			else if (addrToFree > (char*)tail){

				newptr->next = NULL;
				newptr->prev = tail;
				tail->next = newptr;
				tail = newptr;

			}
			//search free list to put node
			//in sequential order
			else{


				while(temp != NULL){
					if( (char*)temp > addrToFree){

						(temp->prev)->next = newptr;
						newptr->prev = temp->prev;
						newptr->next = temp;
						temp->prev = newptr;

						break;
					}
					temp = temp->next;

				}//end while

			}//end else

		}//end else of more than one node
		printf("Freed the Pointer Successfully\n\n");
		//Colesce Memory
		Mem_Coalesce();
		return 0;
	}//end if magic number was valid
	else{
		m_error = E_BAD_POINTER;
 		printf("m_error is %d, E_BAD_POINTER \n",m_error);
		return -1;
	}






}//end Mem_Free()
void Mem_Dump(){

	printf("\n\nFREE LIST:------------------------------------------\n");
	list_t *tmp = freep;
	int count=0;


	do{

		printf("Addr of block is %llx\n", (long long unsigned) tmp);	
		printf("Free Size block[%d]: %d\n", count,tmp->size);
		tmp = tmp->next;
		count++;

	}while (tmp != NULL);

	printf("--------------------------------------------------\n\n");


}//end Mem_Dump




