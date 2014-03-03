CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3 -I../..
LIBS = -lgamecon -lgdi32 -L../..

animation3.exe: animation3.o
	$(CC) $^ -o $@ $(LIBS) -mwindows

animation3.o: animation3.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) animation3.exe animation3.o
