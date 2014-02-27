CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3 -I../..
LIBS = -lgamecon -lgdi32 -L../..

control.exe: control.o
	$(CC) $^ -o $@ $(LIBS) -mwindows

control.o: control.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) control.exe control.o
