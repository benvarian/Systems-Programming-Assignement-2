C = cc 
CSTD = -std=c11 
CFLAGS = -Wall -Werror -pedantic
OBJS = trove.o trovefunc.o
trove : trove.c trovefunc.c
	$(C) $(CSTD) $(CFLAGS) -o trove trove.c

clean : 
	rm -f $(OBJS)
