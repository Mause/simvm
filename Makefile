all: main test assembler

CFLAGS += -g -std=c99

main: main.c vm.h vm.c
	$(CC) $(CFLAGS) main.c vm.c -o main


test: test.c vm.h vm.c
	$(CC) $(CFLAGS) test.c vm.c -o test


assembler: assembler.c
	$(CC) assembler.c -o assembler


clean:
	rm -f main
