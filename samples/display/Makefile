CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3 -I../..
LIBS = -lgamecon -lgdi32 -L../..

display.exe: display.o
	$(CC) $^ -o $@ $(LIBS) -mwindows

display.o: display.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) display.exe display.o
