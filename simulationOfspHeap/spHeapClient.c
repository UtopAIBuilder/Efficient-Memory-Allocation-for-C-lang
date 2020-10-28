#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include "spHeapHeader.h"

int present_time=0;
int total_req_bytes=0;
struct sim_list_node{
    int time;
    void * address;
    struct sim_list_node *next;
};
typedef struct sim_list_node sim_list_node;
sim_list_node *sim_head = NULL;
void timeSortedInsert(sim_list_node* new_node) 
{ 
    sim_list_node* current; 
    if (sim_head == NULL || sim_head->time >= new_node->time) 
    { 
        new_node->next = sim_head; 
        sim_head = new_node; 
    } 
    else
    { 
        current = sim_head; 
        while (current->next!=NULL && 
               current->next->time < new_node->time) 
        { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
} 
sim_list_node *create_node(int time,char *address) 
{ 
    sim_list_node* new_node = (sim_list_node*) malloc(sizeof(sim_list_node)); 
    new_node->time  = time; 
    new_node->address=address;
    new_node->next =  NULL; 
    return new_node; 
} 
void * delete_node(size_t time){
    if (sim_head==NULL){
        return NULL;
    }
    else
    {
        if (sim_head->time>time){
            return NULL;
        }
        else
        {
            void *add=sim_head->address;
            sim_list_node *current=sim_head;
            sim_head=sim_head->next;
            free(current);
            return add;
        }
        
    }

} 

size_t uniformDist(int lower_bound,int upper_bound){
    size_t out;
    out=(rand()%(upper_bound-lower_bound+1))+lower_bound;
    return out;
}
size_t truncExponentialDist(int lower_bound,int upper_bound,int mean){
    float u=(float)rand()/RAND_MAX;
    float p=lower_bound+(-1*mean*log(1-u));
    if(p>upper_bound)
    {
        return upper_bound;
    }
    else
    return p;

}




int main(int argc, char const *argv[]){
    initMEM();
    //size_t tempOffsetArr[100];
    int d = 0;
    int choice=1;
    srand(time(0));
	printf("enter 1 for uniform request,\n\t2 for exponential request\n(if entered some other no. then uniform will processed..)\n");
	scanf("%d",&choice);
    for (int i = 0; i < 10000; i++)
    {
        printASL();
        printf("i:%d\n", i);

        present_time++;
        size_t request_size;
	if(choice==2)
     	request_size = truncExponentialDist(100, 4000,2000);
	else
	request_size = uniformDist(100, 4000);
        printf("request_size:%zu\n", request_size);
        size_t time = uniformDist(0, 100);
        /* void *ptr = delete_node(present_time);
        while (ptr != NULL)
        {
            d = deallocate(ptr);
          //  printf("return of deallocate:%d\n", d);

            ptr = delete_node(present_time);
        } */

        char *address = allocateMEM(request_size);
        printf("allocated address: %zu\n", address - (char *)baseAddress());
        if (address != NULL)
        {
            total_req_bytes += request_size;
            sim_list_node *new = create_node(time + present_time, address);
            timeSortedInsert(new);
            //internal fragmentation track
        }

        else
        {
            printf("external Frag: %f\n",(float)((float)getCurrentlyAvaMem()/(float)getTotalAvaMem()));
            exit(1); //external fragmentation track
        }
    }
    printASL();

    printf("internal frag:%f\n", internalFragMent());
    printf("internalFrag per:%f\n", (internalFragMent()) / total_req_bytes);
    printf("DEALLOCATING all nodes previously allocated:\n");
    sim_list_node *temp;
    while (sim_head)
    {
        deallocate(sim_head->address);
        temp=sim_head;
        sim_head=sim_head->next;
        free(temp);
    }
    printASL();
printf("noOfSplits: %d\n",getCountOfSplits());
printf("noOfMerges: %d\n",getCountOfMerges());
    return 0;
}
