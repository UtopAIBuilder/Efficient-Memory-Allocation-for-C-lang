#include<stdio.h>
#include<stdlib.h>
#include "header.h"
#include "oneBinHeader.h"

int main()
{   
	initOneBin(sizeof(struct polyNode));
	menu();
	return 0;
}

//space=32, = : 61
