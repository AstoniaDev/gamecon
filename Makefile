AR = ar
CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3

libgamecon.a: gamecon.o
	$(AR) rcs $@ $^

gamecon.o: gamecon.c gamecon.h
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) libgamecon.a gamecon.o
