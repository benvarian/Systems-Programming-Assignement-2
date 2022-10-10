C = cc 
PROJECT = trove
CSTD = -std=c11 
CFLAGS = -Wall -Werror -pedantic
trove : trove.o func.o
	$(C) $(CSTD) $(CFLAGS) -o trove \
		trove.o func.o -lm

trove.o : trove.c trove.h
	$(C) $(CSTD) $(CFLAGS) -c trove.c

func.o : func.c trove.h
	$(C) $(CSTD) $(CFLAGS) -c func.c

clean : 
	rm -f $(PROJECT) *.o
