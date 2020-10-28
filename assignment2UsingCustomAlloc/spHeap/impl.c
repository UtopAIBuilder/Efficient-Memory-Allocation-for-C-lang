#include<stdio.h>
#include<stdlib.h>
#include "header.h"
#include <math.h>
#include <limits.h>
#include "spHeapHeader.h"


int printList(struct node* head)
{
 if (!head)
 {
 	printf("empty list\n");
 	return 0;
 }
 if (!(head->next))
 {
 	printf("%04u",head->val);
 	//formatingPrint(head,' ');
 	return 0;
 }

 printList(head->next);
  printf(",%04u",head->val);
  //formatingPrint(head,',');

}








struct node* addition(struct node * input1,struct node* input2)
{

  struct node* result=NULL,*p=NULL,*temp,*del;
  unsigned int sum,carry=0;
  while(input1 && input2)
  {
  	sum=input2->val + input1->val+carry;
  	if (result==NULL)
  	{
  	 result=(struct node *)allocateMEM(sizeof(struct node));
  	 result->val=(sum)%10000;
  	 result->next=NULL;
  	 p=result;
  	 
  	}
  	else
  	{
  		temp= (struct node *)allocateMEM(sizeof(struct node));
  		temp->val=(sum)%10000;
  		temp->next=NULL;
  		p->next=temp;
  		p=temp;
  	}
  	carry=sum/10000;
    del=input1;
    input1=input1->next;
    deallocate(del);
    del=input2;
    input2=input2->next;
    deallocate(del);
  	
 }


if (input1)
{
	p->next=input1;
	
	while(input1 && carry!=0)
 	{
  	sum=input1->val+carry;
  	input1->val=sum%10000;
  	temp=input1;
  	input1=input1->next;
  	carry=sum/10000;

 	}

 	if (carry==0)
 	{
 		return result;
 	}
 	else
 	{
      temp->next=(struct node *)allocateMEM(sizeof(struct node));
      temp=temp->next;
      temp->val=carry;
      temp->next=NULL;
      return result;
 	}
}

if (input2)
{
	p->next=input2;

	while(input2 && carry!=0)
 	{
  	sum=input2->val+carry;
  	input2->val=sum%10000;
  	temp=input2;
  	input2=input2->next;
  	carry=sum/10000;

 	}

 	if (carry==0)
 	{
 		return result;
 	}
 	else
 	{
      temp->next=(struct node *)allocateMEM(sizeof(struct node));
      temp=temp->next;
      temp->val=carry;
      temp->next=NULL;
      return result;
 	}
}

if (carry!=0)
{
p->next=(struct node *)allocateMEM(sizeof(struct node));
p=p->next;
p->val=carry;
p->next=NULL;
return result;
}

  return result;
}





struct node* mul(struct node * input1,struct node* input2)
{
	struct node* temp1= NULL,*result=NULL,*temp,*add1=NULL,*add2=NULL,*p;
	unsigned int sum,carry,count=0;
	while(input2)
	{
		temp1=input1;
		carry=0;

		while(temp1)
		{
          sum=temp1->val * input2->val + carry;
         //printf("sum:%u carry:%u\n",sum,carry);
         if (result==NULL)
  			{
  	 			result=(struct node *)allocateMEM(sizeof(struct node));
  	 			result->val=(sum)%10000;
  	 			result->next=NULL;
  	 			p=result;
  	 
  			}
  		else
  			{
  				temp= (struct node *)allocateMEM(sizeof(struct node));
  				temp->val=(sum)%10000;
  				temp->next=NULL;
  				p->next=temp;
  				p=temp;
  				//printf("result(lsb):%u\n",p->val);
  			}
  			carry=sum/10000;
  			temp1=temp1->next;


		}

		if (carry==0)
  			{
  				
  				if (add1==NULL)
  				{
  					add1=result;
  					
  				}
  				else
  				{

  					add2=result;
  					for (int i = 0; i < count; ++i)
  					{
  						temp= (struct node *)allocateMEM(sizeof(struct node));
  						temp->val=0;
  						temp->next=add2;
  						add2=temp;
  					}
  				}
  				result=NULL;
  				count++;
  				if (add1 && add2)
  				{
  					
  					add1=addition(add1,add2);
  				
  				}
  			}

  		else
  		{
  			p->next=(struct node *)allocateMEM(sizeof(struct node));
  			p=p->next;
			p->val=carry;
			p->next=NULL;
			
			if (add1==NULL)
  				{
  					add1=result;
  					
  				}
  				else
  				{

  					add2=result;
  					for (int i = 0; i < count; ++i)
  					{
  						temp= (struct node *)allocateMEM(sizeof(struct node));
  						temp->val=0;
  						temp->next=add2;
  						add2=temp;
  					}
  				}
  				result=NULL;
  				count++;
  				if (add1 && add2)
  				{ 
  					
  					add1=addition(add1,add2);
  					
  				}
  		}

		input2=input2->next;
	}

	return add1;
}

struct node* expressionEval()
{
  struct node *link1=NULL,*link2=NULL;
  char opCode='@';  //'=':'+':'*' 
    link1=inputTry();

  while(1)
  {
  	scanf("%c",&opCode);
  	if (opCode=='=')
  	{
  		break;
  	}
  	else if (opCode=='+')
  	{
		link2=inputTry();
		link1=addition(link1,link2);
  	}
  	else if (opCode=='*')
  	{
		link2=inputTry();
		link1=mul(link1,link2);
  	}
  	else
  	{
  		printf("expression is not in right format. terminating the program....\n");
  		exit(0);
  	}


  }

  return link1;
}

struct polyNode* inputPoly()
{
	int i=-1;
	float a;
	char c='@';
	int iprev= INT_MAX;
	//printf("%d\n",iprev);
	int dummyNodes=0,dummyNodesdeg;
	struct polyNode* head=NULL;
	struct polyNode* p=NULL;

	do{
		scanf("(%d,%f)%c",&i,&a,&c);
		if (c!=',' && c!='\n' || i>=iprev || i<0)
		{
			printf("%c %d %d\n",c,i,iprev);
			printf("invalid input\nterminating the program....\n");
			exit(0);
		}
		if (iprev!=INT_MAX)
		{
			dummyNodes=iprev-i-1;
			dummyNodesdeg=iprev-1;
		}
		//printf("%d\n",iprev);
		iprev=i;
		//printf("%d\n",iprev);
		
		for (int i = 0; i <dummyNodes; ++i)
		{
			
   			p->next =(struct polyNode*)allocateMEM(sizeof(struct polyNode));
   			p->next->prev=p;
   			p=p->next;
   			p->next=NULL;
   			p->deg=dummyNodesdeg--;
   	 		p->coef=0;
		}

   		if (head==NULL)
   			{
   	 		head=(struct polyNode*)allocateMEM(sizeof(struct polyNode));
   	 		head->next=NULL;
   	 		head->prev=NULL;
   	 		p=head;
   			}
   		else
   			{
   			p->next =(struct polyNode*)allocateMEM(sizeof(struct polyNode));
   			p->next->prev=p;
   			p=p->next;
   			p->next=NULL;
   			}
   			p->deg=i;
   	 		p->coef=a;
		
	}while(c!='\n');

	if (iprev!=INT_MAX)
		{
			dummyNodes=i;
			dummyNodesdeg=i-1;
		}
		
		for (int i = 0; i <dummyNodes; ++i)
		{
			
   			p->next =(struct polyNode*)allocateMEM(sizeof(struct polyNode));
   			p->next->prev=p;
   			p=p->next;
   			p->next=NULL;
   			p->deg=dummyNodesdeg--;
   	 		p->coef=0;
		}


	return head;

}

int printPoly(struct polyNode* head)
{
	int flag=0;
	if (head==NULL)
	{
		printf("(0,0.000000)\n");
		//printf("list is empty\n");
		return 0;
	}
  while(head->next!=NULL)
  {
  	if (head->coef!=0.000000 && head->coef!=-0.000000)
  	{
  		if (flag==0)
  		{
  			printf("(%d,%f)",head->deg,head->coef);
  			flag=1;
  		}
  		else
  		{
  			printf(",(%d,%f)",head->deg,head->coef);
  		}
  		
  	}
  	head=head->next;
  }
  if (head->coef!=0.000000 && head->coef!=-0.000000)
  	{
  		if (flag==0)
  		{
  			printf("(%d,%f)",head->deg,head->coef);
  			flag=1;
  		}
  		else
  		{
  			printf(",(%d,%f)",head->deg,head->coef);
  		}
  	}

  	if (flag==0)
  		{
  			printf("(0,0.000000)");
  			flag=1;
  		}
  		
  printf("\n");
  return 1;
}

void polyDiv(struct polyNode* divisor,struct polyNode* dividend)
{
	struct polyNode* quotient=NULL,*tempDivisor=divisor,*tempQuotient=NULL,*tempremainder=dividend,*p1,*p2;
	float bn=divisor->coef;

   for (int i = dividend->deg - divisor->deg; i >=0 ; --i)
   {
   	if (quotient==NULL)
   	{
   		quotient=(struct polyNode*)allocateMEM(sizeof(struct polyNode));
   		quotient->prev=NULL;
   		quotient->next=NULL;
   		tempQuotient=quotient;
	}
	else
	{
		tempQuotient->next =(struct polyNode*)allocateMEM(sizeof(struct polyNode));
   		tempQuotient->next->prev=tempQuotient;
   		tempQuotient=tempQuotient->next;
   		tempQuotient->next=NULL;
	}

	tempQuotient->deg=i;
	tempQuotient->coef= (tempremainder->coef)/bn;
	p1=tempremainder->next;
	p2=divisor->next;
	for (int j = divisor->deg;j>0; --j)
	{
		p1->coef= p1->coef - (tempQuotient->coef)*(p2->coef);
		p1=p1->next;
		p2=p2->next;
	}
	tempremainder = tempremainder->next;

   	
   }

   printf("quotient:");
   printPoly(quotient);
   printf("remainder:");
   printPoly(tempremainder);
   deletePolylist(quotient);
   deletePolylist(divisor);
   deletePolylist(dividend);
}


void menu()
{
    struct node * link1,*link2;
	int choice;
	int star=50;	
	printf("\n\n");
			while(star--)
				printf("*");
	printf("\n");
	printf("choose a function to be checked for its correctness\n\n");
	printf("1 large integer expression evaluation\n");
	printf("2 polynomial division\n");
	printf("3 exit\n");
	char ppp;
	scanf("%d%c",&choice,&ppp);

	switch(choice)
	{
		case 1: 
		{
			printf("input expression in proper format(no spaces in between)\n");
			struct node*result=expressionEval();
			printf("output(expressionVal): ");
			printList(result);
			printf("$\n");
			menu();
			break;
		}
		case 2: 
		{
			//printf("inputDividend:");
    		struct polyNode* alpha=inputPoly();
    		//printf("inputDivisor:");
    		struct polyNode* beta=inputPoly();
    		polyDiv(beta,alpha);
    		menu();
    		break;
		}
		case 3:
		{
			printf("exiting the program. thank you.\n");
			printf("internalFragment proportion in (0,1):%f\n",internalFragMent());
			printf("size of polyNode:%zu",sizeof(struct polyNode));
			exit(0);
			break;
		}
		default:
		{
			printf("user input can't be recognized. Try again with different choice from {1,2,3\n");
			menu();
			break;
		}
				
	}
}

void deletePolylist(struct polyNode* head )
{
	struct polyNode * temp=head;
  while(head!=NULL)
  {
    temp=head;
    head=head->next;
    deallocate(temp);
  }
}


struct node* inputTry()
{
	unsigned int digit=-1;
	char sep='@';

  struct node * head=NULL;
  struct node * temp=NULL;
	do
	{
		scanf("%u%c",&digit,&sep);
		if (digit & 1<<31 || digit>9999 || sep!=',' && sep!='$')
		{
			printf("invalid input. exiting the program\n");
			exit(0);
		}

		if(head==NULL)
    {
       head = (struct node *)allocateMEM(sizeof(struct node));
       head->val=digit;
       head->next=NULL;
    }

   
	else
	{
      temp= (struct node *)allocateMEM(sizeof(struct node));
      temp->val=digit;
      temp->next=head;
      head=temp;
	}
	} while(sep!='$');

	return head;
}

