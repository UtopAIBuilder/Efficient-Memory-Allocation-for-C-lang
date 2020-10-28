#include<stdio.h>
struct free_list_node
{
    void * address;
    struct free_list_node * next;
    struct free_list_node * previous;
};
typedef struct free_list_node free_list_node;
struct ASL_ele
{
    size_t size;
    free_list_node *head;
    free_list_node *tail;
};
typedef struct ASL_ele ASL_ele;
struct parent_list
{
    size_t address;
    int parent_type;
    size_t size;
   struct parent_list*next;
};
typedef struct parent_list parent_list;
void* initMEM();
void memNotAvail();
free_list_node* createNode(void*address);
void setTAG(size_t offset_val,int tag_val);
int getTAG(size_t offset_val);
void setKVAL(size_t offset_val,int kval);
int getKVAL(size_t offset_val);
void setTYPE(size_t offset_val,int type);
int getTYPE(size_t offset_val);
void* allocateMEM(size_t bytes_req);
int split(int index);
void insertNodeAt(int index,free_list_node* node);
void genericSplitCore(int type1,int type2,int coeff,int*temp_index,int index);
void* deleteNode(int index);
void printASL();
void deleteBuddy(size_t budy_offset,int coeff,int kval);
size_t merge(size_t temp_offset,size_t buddy_offset,int kval,int coeff);
size_t findBuddy(size_t temp_offset);
int deallocate(void* address);
float internalFragMent();
void* baseAddress();
int getCountOfSplits();
int getCountOfMerges();
size_t getCurrentlyAvaMem();
size_t getTotalAvaMem();