a.out: client.o impl.o spHeapImpl.o
	gcc client.o impl.o spHeapImpl.o -lm

client.o: client.c header.h spHeapHeader.h
	gcc -c client.c -lm

impl.o: impl.c header.h spHeapHeader.h
	gcc -c impl.c -lm

spHeapImpl.o: spHeapImpl.c spHeapHeader.h
	gcc -c spHeapImpl.c -lm
