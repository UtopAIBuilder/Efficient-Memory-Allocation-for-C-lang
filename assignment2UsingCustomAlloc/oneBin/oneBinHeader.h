#include<stdio.h>
void initOneBin(size_t size_of_block);
void* allocate();
int deallocate(void *address);
/* struct free_list_node
{
    void * address;
    struct free_list_node * next;
};
typedef struct free_list_node free_list_node; */
//void printASL();
void* baseAddress();