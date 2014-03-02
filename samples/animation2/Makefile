CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3 -I../..
LIBS = -lgamecon -lgdi32 -L../..

animation2.exe: animation2.o
	$(CC) $^ -o $@ $(LIBS) -mwindows

animation2.o: animation2.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) animation2.exe animation2.o
