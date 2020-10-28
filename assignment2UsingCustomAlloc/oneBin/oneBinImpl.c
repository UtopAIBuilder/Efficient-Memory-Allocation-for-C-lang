#include<stdio.h>
#include "oneBinHeader.h"
#include<stdlib.h>
#define m 28
//free_list_node* head=NULL;
void* base_address;
int* tag_array;
size_t block_size=1;
int init_var=0;
int no_of_blocks;
int last_alloc_index=-1;
void initOneBin(size_t size_of_block)
{
    if(size_of_block>(1<<m) || size_of_block==0)
    {
        printf("size of block is expected in the range of [0,%d]... try again..\n",(1<<m));
        exit(1);
    }
    block_size=size_of_block;
    no_of_blocks=(1<<m)/size_of_block;
    tag_array=(int*)calloc(no_of_blocks,sizeof(int));
    base_address=malloc(1<<m);
    init_var=1;
    //free_list_node *temp;
    
   /*  for(int i=0;i<no_of_blocks;i++)
    {
        temp=(free_list_node*)malloc(sizeof(free_list_node));
        temp->address=(char*)base_address + size_of_block*i;
        temp->next=head;
        head=temp;
    } */
}
void* allocate()
{
    if(!init_var)
    {
        printf("memory has not been initialised properly... try again...\n");
        exit(1);
    }
    int i=-1;
    if(!tag_array[++last_alloc_index])
        i=last_alloc_index-1;
    while (i<no_of_blocks-1 && tag_array[++i])
    {
    }
    if(tag_array[i])
    {
        printf("mem not avail... returning NULL\n");
        return NULL;
    }
    void* return_address=(char*)base_address + i*block_size;
  /*   free_list_node* temp=head;
    head=head->next;
    free(temp); */
    tag_array[i]=1;
    last_alloc_index=i;
    return return_address;
}
int deallocate(void*address)
{
    size_t offset=(char*)address-(char*)base_address;
    if((offset)%block_size!=0 || tag_array[offset/block_size]==0)
    {
        printf("invalid address.. deallocation failed\n");
        return 0;
    }
    /* free_list_node* temp=(free_list_node*)malloc(sizeof(free_list_node));
    temp->next=head;
    temp->address=address;
    head=temp; */
    tag_array[offset/block_size]=0;
    return 1;
}
/* void printASL()
{
    free_list_node *temp=head;
    printf("addresses: ");
    while (temp)
    {
        printf("[%zu]:",(char*)temp->address-(char*)base_address);
        temp=temp->next;
    }
    printf("\n");
    
} */
void* baseAddress()
{
    return base_address;
}

