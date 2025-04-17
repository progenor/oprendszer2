#make allomany a elso es masodik allomanyok forditasahoz
CFLAGS=-Wall

elso: elso.o
	gcc $(CFLAGS) -o elso  elso.o

elso.o: elso.c
	gcc $(CFLAGS) -c elso.c

masodik: masodik.o
	gcc $(CFLAGS) -o masodik masodik.o

masodik.o: masodik.c 
	gcc $(CFLAGS) -c masodik.c

all: elso masodik 

clean:   
	rm -f *.o masodik elso 
