all: main test assembler

CFLAGS = -g -std=c99 -pedantic -Wall -D_XOPEN_SOURCE=500 -D_CRT_SECURE_NO_WARNINGS

main: main.c vm.h vm.c
	$(CC) $(CFLAGS) main.c vm.c -o main


test: test.c vm.h vm.c
	$(CC) $(CFLAGS) test.c vm.c -o test


assembler: assembler.c
	$(CC) $(CFLAGS) assembler.c -o assembler


REMOVE = main test assembler *.obj *.bin *.exe


clean:
	rm -f $(REMOVE)


clean_win:
	del $(REMOVE)
