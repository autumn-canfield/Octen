CC=gcc
CFLAGS= -c -Iheaders -Wall -Wextra -std=gnu99 -D DEVELOP
OFILES = hex.o hash.o table.o memory.o octen.o ipc.o
LIBFILES = table.o ipc.o memory.o library.o
LDFLAGS= #-lws2_32
LIBNAME = library/lib/libocten.a
EXAMPLEFLAGS = -Llibrary/lib -Ilibrary/include -locten
ifeq ($(OS),Windows_NT)
	ONAME = octen.exe
	EXAMPLENAME = example.exe
else
	ONAME = octen
	EXAMPLENAME = example
endif

all:
	$(CC) $(CFLAGS) hex.c
	$(CC) $(CFLAGS) hash.c
	$(CC) $(CFLAGS) table.c
	$(CC) $(CFLAGS) memory.c
	$(CC) $(CFLAGS) ipc.c
	$(CC) $(CFLAGS) octen.c
	$(CC) $(CFLAGS) library.c
	$(CC) $(OFILES) $(LDFLAGS) -o $(ONAME)
	mkdir -p library/lib
	ar rcs $(LIBNAME) $(LIBFILES)
	$(CC) library/example.c $(EXAMPLEFLAGS) -o $(EXAMPLENAME)

clean:
	rm -rf *.o

