COMP = gcc
TARGET = main
CFLAGS = -ansi -pedantic -Wall -Werror
INCDIR = -I../include/
CK = -c
LDK = -o
FILES = main.c mem_list.c mem.c
OFILES = main.o mem_list.o mem.o

main: main.c
	$(COMP) $(CK) $(FILES) $(INCDIR)
	$(COMP) $(LDK) $(TARGET) $(CFLAGS) $(OFILES) -lm

clean:
	rm -f $(TARGET) *.o
