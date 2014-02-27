CC = gcc
RM = del
CCFLAGS = -Wall -Wpointer-arith -O3 -I../..
LIBS = -lgamecon -lgdi32 -L../..

text.exe: text.o
	$(CC) $^ -o $@ $(LIBS) -mwindows

text.o: text.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) text.exe text.o
