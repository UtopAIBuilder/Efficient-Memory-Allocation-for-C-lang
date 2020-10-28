a.out: spHeapClient.o spHeapImpl.o
	gcc -g spHeapClient.o spHeapImpl.o -lm

spHeapClient.o: spHeapClient.c spHeapHeader.h
	gcc -g -c spHeapClient.c -lm

spHeapImpl.o: spHeapImpl.c spHeapHeader.h
	gcc -g -c spHeapImpl.c -lm
