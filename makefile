C = cc 
PROJECT = trove
CSTD = -std=c11 
CFLAGS = -Wall -Werror -pedantic
trove : trove.o func.o hash.o
	$(C) $(CSTD) $(CFLAGS) -o trove \
		trove.o func.o hash.o -lm

trove.o : trove.c trove.h
	$(C) $(CSTD) $(CFLAGS) -c trove.c

func.o : func.c trove.h
	$(C) $(CSTD) $(CFLAGS) -c func.c
	
hash.o : hash.c trove.h
	$(C) $(CSTD) $(CFLAGS) -c hash.c

clean : 
	rm -f $(PROJECT) *.o
