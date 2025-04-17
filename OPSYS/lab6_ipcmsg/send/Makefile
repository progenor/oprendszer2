CFLAGS=-Wall -ggdb

recv: recv.o
	gcc $(CFLAGS) -o recv  recv.o

recv.o: recv.c
	gcc $(CFLAGS) -c recv.c

send: send.o
	gcc $(CFLAGS) -o send send.o

send.o: send.c 
	gcc $(CFLAGS) -c send.c

check: check.o
	gcc $(CFLAGS) -o check check.o

check.o: check.c
	gcc $(CFLAGS) -c check.c

remq: remq.o
	gcc $(CFLAGS) -o remq remq.o

remq.o: remq.c
	gcc $(CFLAGS) -c remq.c

all: recv send check remq

clean:   
	rm -f *.o send recv check remq
