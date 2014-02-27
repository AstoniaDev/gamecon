CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3 -I../..
LIBS = -lgamecon -lgdi32 -L../..

animation.exe: animation.o
	$(CC) $^ -o $@ $(LIBS) -mwindows

animation.o: animation.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) animation.exe animation.o
