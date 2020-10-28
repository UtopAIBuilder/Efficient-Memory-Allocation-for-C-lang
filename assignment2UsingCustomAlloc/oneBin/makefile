a.out: client.o impl.o oneBinImpl.o
	gcc client.o impl.o oneBinImpl.o -lm

client.o: client.c header.h oneBinHeader.h
	gcc -c client.c -lm

impl.o: impl.c header.h oneBinHeader.h
	gcc -c impl.c -lm

oneBinImpl.o: oneBinImpl.c oneBinHeader.h
	gcc -c oneBinImpl.c -lm
