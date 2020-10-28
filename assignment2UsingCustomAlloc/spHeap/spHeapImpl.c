#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "spHeapHeader.h"
#include<limits.h>
#define m 28
ASL_ele ASL[2 * m];
char *CMD;
void *base_address;
size_t available_mem;
float internal_frag;
parent_list *head=NULL;
void *initMEM() // memory initialization...
{
    for (int i = 0; i < 2 * m; i++)
    {
        ASL[i].head = NULL;
        ASL[i].tail = NULL;
        if (i % 2)
            ASL[i].size = 1<<((i+1)/2);
           // ASL[i].size = pow(2, (i + 1) / 2);
        else
            ASL[i].size = 3 *(1<<(i/2-1));
           // ASL[i].size = 3 * pow(2, i / 2 - 1);
    }
    base_address = malloc(1<<m);
    //base_address = malloc(pow(2, m));
    if (base_address == NULL)
        memNotAvail();
    free_list_node *base_block = createNode(base_address);
    ASL[2 * m - 1].head = base_block;
    ASL[2 * m - 1].tail = base_block;
    available_mem = 1<<m;
    //available_mem = pow(2, 28);
    internal_frag = 0;
    CMD = (char *)calloc(1<<m, 1);
    //CMD = (char *)calloc(pow(2, 28), 1);
    if (CMD == NULL)
        memNotAvail();
    setTAG(0, 0);
    setKVAL(0, m);
  //  printf("assigned kval:%d\n",getKVAL(0));
    setTYPE(0, 0);
}

void memNotAvail()
{
    printf("memory not available..");
    exit(1);
}

free_list_node *createNode(void *address)
{
    free_list_node *temp = (free_list_node *)malloc(sizeof(free_list_node));
    if (temp == NULL)
        memNotAvail();
    temp->address = address;
    temp->next = NULL;
    temp->previous = NULL;
    return temp;
}
void setTAG(size_t offset_val, int tag_val)
{
   // if(offset_val>pow(2,m)|| offset_val<0)
    if(offset_val>(1<<m)|| offset_val<0)
    return;
    CMD[offset_val] = (CMD[offset_val] & (0x7F)) | (tag_val << 7);
}
int getTAG(size_t offset_val)
{
    //if(offset_val>pow(2,m)|| offset_val<0)
    if(offset_val>(1<<m)|| offset_val<0)
    return -1;
    return (int)(CMD[offset_val]>>7 & (0x01));
}
void setKVAL(size_t offset_val, int kval)
{
    //if(offset_val>pow(2,m)|| offset_val<0)
    if(offset_val>(1<<m)|| offset_val<0)
    return;
    CMD[offset_val] = (CMD[offset_val] & (0x83)) | (kval << 2);
  //  printf("CMD[offset_val]:%d\n",CMD[offset_val]);
}
int getKVAL(size_t offset_val)
{
   // if(offset_val>pow(2,m)|| offset_val<0)
    if(offset_val>(1<<m)|| offset_val<0)
    return -1;
    return (int)(CMD[offset_val]>>2 & (0x1f));
}
void setTYPE(size_t offset_val, int type)
{
   // if(offset_val>pow(2,m)|| offset_val<0)
   if(offset_val>(1<<m)|| offset_val<0)
    return;
    CMD[offset_val] = (CMD[offset_val] & (0xFC)) | (type);
}
int getTYPE(size_t offset_val)
{
   // if(offset_val>pow(2,m)|| offset_val<0)
   if(offset_val>(1<<m)|| offset_val<0)
    return -1;
    return (int)(CMD[offset_val] & (0x03));
}
void setParentType(size_t address,size_t size)
{
    parent_list *temp;
    temp = (parent_list *)malloc(sizeof(parent_list));
    temp->address = address;
    temp->size=size;
    temp->parent_type = getTYPE(address);
    temp->next = head;
    head = temp;
}
int getParentType(size_t address,size_t size)
{
    parent_list*temp=head;
    parent_list*prev=NULL;
    while (temp!=NULL && (temp->address!=address || temp->size!=size))
    {
        prev=temp;
        temp=temp->next;
    }
    if(temp==NULL)
    {
        printf("parent type list does not have such entry...\n");
        return 0;
    }
    if(prev)
    prev->next=temp->next;
    else
    head=head->next;
    int type=temp->parent_type;
    free(temp);
    return type;
}

void *allocateMEM(size_t bytes_req)
{
    void* return_address;
    //if (bytes_req > pow(2, m) || bytes_req <= 0)
    if (bytes_req > (1<<m) || bytes_req <= 0)
    {
        printf("mem can't be availed(total avail mem is %zu BYTES)... returning NULL\n", available_mem);
        return NULL;
    }
    int index;
    if (bytes_req == 1)
        index = 0;
    else
    {
        index = 2 * ceil(log2(bytes_req)) - 1;
        if (ASL[index - 1].size >= bytes_req)
            index = index - 1;
    }

    if (ASL[index].head)
    {
        available_mem = available_mem - ASL[index].size;
        internal_frag += (float)(ASL[index].size - bytes_req) ;
        //printf("internal frag:%lf\n",internalFragMent());
        if(ASL[index].head==NULL)
        printf("1.index:%d in if of memAlloc\n",index);
        return_address=deleteNode(index);
        return return_address;
    }
    else
    {
        int avail_flag = split(index);
        if (!avail_flag)
        {
            printf("mem can't be availed(total avail mem is %zu BYTES)... returning NULL\n", available_mem);
            return NULL;
        }
        available_mem = available_mem - ASL[index].size;
        internal_frag +=(float) (ASL[index].size - bytes_req);
        //printf("internal frag:%lf\n",internalFragMent());
        if(ASL[index].head==NULL)
        printf("index:%d in else of memAlloc\n",index);
        return_address=deleteNode(index);
        return return_address;
    }
}

int split(int index)
{
    int temp_index = index;
    int split_count=0;
  //  printf("2.splitting for index:%d\n",index);
    if(index==0 && ASL[1].head!=NULL)
    {
        temp_index=1;
    }
    while (temp_index < 2 * m - 1 && ASL[++temp_index].head == NULL)
    {
    } // finding the smallest avail block...after index 1..

    if (ASL[temp_index].head == NULL)
    {
        if(index==0 && ASL[1].head!=NULL )
        {
            void* address=deleteNode(1);
            free_list_node* temp=createNode(address);
            insertNodeAt(0,temp);
            available_mem=available_mem-1;
            return 1;
        }
        //printf("2:mem can't be availed(total avail mem is %zu BYTES)... returning NULL\n", available_mem);
        return 0;
    }
    while (ASL[index].head == NULL)
    {
       // printf("split count:%d\n",split_count++);
        if (ASL[temp_index].size % 3 == 0)
        {
            genericSplitCore(1, 2, 3, &temp_index, index);
            /* kval_source=log2(ASL[temp_index].size/3);
            source_address=deleteNode(temp_index);
            temp1_offset=(char*)source_address-(char*)base_address;
            temp2_offset=temp1_offset+pow(2,kval_source+1);
            setTAG(temp1_offset,0);
            setTAG(temp2_offset,0);
            setKVAL(temp1_offset,1+kval_source);
            setKVAL(temp2_offset,kval_source);
            setTYPE(temp1_offset,1);
            setTYPE(temp2_offset,2);
            temp1=createNode(source_address);
            temp2=createNode(temp2_offset+(char*)base_address);
            insertNodeAt(temp_index-1,temp1);
            idx2=temp_index-3;
            if(idx2<0)
            idx2=0;
            insertNodeAt(idx2,temp2);
            if(ASL[idx2].size>ASL[index].size)
            temp_index=idx2;
            else
            temp_index--; */
        }
        else //dividing block of the form 2^k...
        {
            genericSplitCore(3, 3, 1, &temp_index, index);
        }
    }
      return 1;
}
void insertNodeAt(int index, free_list_node *node)
{
    if (ASL[index].tail)
    {
        ASL[index].tail->next = node;
        node->previous = ASL[index].tail;
        ASL[index].tail = node;
    }
    else
    {
        ASL[index].tail = node;
        ASL[index].head = node;
    }
}
void genericSplitCore(int type1, int type2, int coeff, int *temp_index, int index)
{
    //printf("temp_index:%d in generic split core\n",*temp_index);
    int kval_source;
    int idx2;
    void *source_address;
    size_t temp1_offset, temp2_offset;
    free_list_node *temp1, *temp2;
    kval_source = log2(ASL[*temp_index].size / coeff);
    if(ASL[*temp_index].head==NULL)
        printf("temp_index:%d in generic split core\n",*temp_index);
    source_address = deleteNode(*temp_index);
    temp1_offset = (char *)source_address - (char *)base_address;
    if(coeff==1)
    setParentType(temp1_offset,ASL[*temp_index].size);
    //temp2_offset = temp1_offset + (3 / coeff) * pow(2, kval_source + (3 * coeff - 7) / 2);
    temp2_offset = temp1_offset + (3 / coeff) * (1<<(kval_source + (3 * coeff - 7) / 2));
    setTAG(temp1_offset, 0);
    setTAG(temp2_offset, 0);
    setKVAL(temp1_offset, kval_source + (3 * coeff - 7) / 2);
    setKVAL(temp2_offset, kval_source + (3 - coeff) * (-1));
    setTYPE(temp1_offset, type1);
    setTYPE(temp2_offset, type2);
    temp1 = createNode(source_address);
    temp2 = createNode(temp2_offset + (char *)base_address);
    insertNodeAt(*temp_index - 1, temp1);
    idx2 = *temp_index - (9 - coeff) / 2;
    if (idx2 < 0)
        idx2 = 0;
    insertNodeAt(idx2, temp2);
    if (ASL[idx2].size > ASL[index].size && ASL[idx2].size!=2)
        *temp_index = idx2;
    else
        (*temp_index)--;
}
void *deleteNode(int index)
{
    free_list_node *t;
    void *address;
    free_list_node *temp = ASL[index].head;
    void *source_address = temp->address;
    size_t offset = (char *)source_address - (char *)base_address;
    ASL[index].head = temp->next;
    if (temp->next)
        temp->next->previous = NULL;
    else
        ASL[index].tail = NULL;
    address = temp->address;
    free(temp);
    setTAG(offset, 1);
    return address;
}

void printASL()
{
    free_list_node *temp;
    size_t size_of_list,totalMemSUM =0;
    for (int i = 0; i < 2 * m; i++)
    {
        size_of_list = 0;
        temp = ASL[i].head;
        printf("index: %3d ,size: %9zu addresses ", i, ASL[i].size);

        while (temp)
        {
            printf(":[%zu]", ((char*)temp->address-(char*)base_address));
            size_of_list += ASL[i].size;
            temp = temp->next;
        }
        //printf(" Bytes_in_list: %zu\n", size_of_list);
        printf("\n");
        totalMemSUM+=size_of_list;
    }
    printf(" total avail_mem: %zu,total_mem_sum:%zu and base_address: <%p>\n", available_mem,totalMemSUM, base_address);

}

int deallocate(void* address)
{
    size_t new_offset;
    size_t t_offset=(char*)address-(char*)base_address;
    int kval=getKVAL(t_offset);
   // if (getTYPE(t_offset)==3 && t_offset % (int)pow(2, kval + 2) == 0)
    if (getTYPE(t_offset)==3 && t_offset % (int)(1<<(kval+2)) == 0)
    {
        //available_mem=available_mem+3*(int)pow(2,kval);
        available_mem=available_mem+3*(int)(1<<kval);
    }
    else
    available_mem=available_mem+(int)(1<<kval);
    //available_mem=available_mem+(int)pow(2,kval);
    
    if(getTAG(t_offset)!=1)
    {
        printf("node has been freed already... or an invalid address..\n");
        return 0;
    }
    free_list_node* newNode;
    do
    {
        new_offset=t_offset;
        t_offset=findBuddy(t_offset); //returns newOffset,setsCMD(newOffset),deletes buddy...
        
    }while(t_offset!=INT_MAX);
    newNode=createNode(new_offset+(char*)base_address);
    kval=getKVAL(new_offset);
    setTAG(new_offset,0);
    //if(getTYPE(new_offset)!=3 || new_offset% (int)pow(2,kval+2)!=0)
    if(getTYPE(new_offset)!=3 || new_offset% (int)(1<<(kval+2))!=0)
    {
        insertNodeAt(2*kval-1,newNode);
    }
    else
    insertNodeAt(2*kval+2,newNode);
    return 1;
}
size_t findBuddy(size_t temp_offset)
{
    int type = getTYPE(temp_offset);
    int kval = getKVAL(temp_offset);
    size_t buddy_offset;
    size_t new_offset;
    if (type == 3)
    {
        if (temp_offset % (int)pow(2, kval + 2) == 0)
        {
            //buddy_offset=temp_offset + 3 * (int)pow(2, kval);
            buddy_offset=temp_offset + 3 * (int)(1<<kval);
            if (getTAG(buddy_offset) == 0 && getKVAL(buddy_offset)==kval && getTYPE(buddy_offset)==3)
            {
                //printf("tempOffset:%zu,budyyOffset:%zu\n",temp_offset,buddy_offset);
                deleteBuddy(buddy_offset,1,kval);
                return merge(temp_offset,buddy_offset, kval + 2,1);
            }
        }
        else
        {
            //buddy_offset=temp_offset - 3 * (int)pow(2, kval);
            buddy_offset=temp_offset - 3 * (int)(1<<kval);
            if (getTAG(buddy_offset) == 0 && getKVAL(buddy_offset)==kval && getTYPE(buddy_offset)==3)
            {
                // printf("tempOffset:%zu,budyyOffset:%zu\n",temp_offset,buddy_offset);
                deleteBuddy(buddy_offset,3,kval);
                return merge(temp_offset,buddy_offset, kval + 2,1);
            }
        }
    }
    else if (type == 1)
    {
        //buddy_offset=temp_offset + (int)pow(2, kval);
        buddy_offset=temp_offset + (int)(1<<kval);
        if (getTAG(buddy_offset) == 0 && getKVAL(buddy_offset)==kval-1 && getTYPE(buddy_offset)==2)
        {
            // printf("tempOffset:%zu,budyyOffset:%zu\n",temp_offset,buddy_offset);
            deleteBuddy(buddy_offset,1,kval-1);
            return merge(temp_offset,buddy_offset, kval - 1,3);
        }
    }
    else if (type == 2)
    {
       // buddy_offset=temp_offset - (int)pow(2, kval + 1);
        buddy_offset=temp_offset - (int)(1<<(kval+1));
        if (getTAG(buddy_offset) == 0 && getKVAL(buddy_offset)==kval+1 && getTYPE(buddy_offset)==1)
        {
            // printf("tempOffset:%zu,budyyOffset:%zu\n",temp_offset,buddy_offset);
            deleteBuddy(buddy_offset,1,kval+1);
            return merge(temp_offset, buddy_offset, kval,3);
        }
    }
    return INT_MAX;
}
size_t merge(size_t temp_offset,size_t buddy_offset,int kval,int coeff)
{
    size_t new_offset;
    if(temp_offset>buddy_offset)
    new_offset=buddy_offset;
    else
    new_offset=temp_offset;
    int type=getTYPE(buddy_offset);
    if(type==3)
    {
         //setTYPE(new_offset,getParentType(new_offset,coeff*pow(2,kval)));
         setTYPE(new_offset,getParentType(new_offset,coeff*(1<<kval)));
    }
    else
    setTYPE(new_offset,3);
    //setTAG(new_offset,0);
    setKVAL(new_offset,kval);
    return new_offset;

}
void deleteBuddy(size_t budy_offset,int coeff,int kval)
{
    int index;
    if(coeff==3)
    {
        index=2*kval+2;
    }
    else
    index=2*kval-1;
    free_list_node* temp=ASL[index].head;
    free_list_node*prev=NULL;
    while(temp!=NULL && temp->address!=budy_offset+(char*)base_address)
    {
        prev=temp;
        temp=temp->next;
    }
    if(temp==NULL)
    {
        printf("something is messed up.. buddy not in the given index..\n");
        exit(1);
    }
    if(prev==NULL)
    {
        ASL[index].head=temp->next;
    }
    else
    prev->next=temp->next;

    if (temp->next)
    {
       temp->next->previous=prev;
    }
    else
    ASL[index].tail=prev;
    free(temp);  
}
float internalFragMent()
{
    return internal_frag;
}
void* baseAddress()
{
    return base_address;
} 
