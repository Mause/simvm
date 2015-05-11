all: main assembler

CFLAGS += -g -std=c99

main: main.c vm.h vm.c
	$(CC) $(CFLAGS) main.c vm.c -o main


assembler: assembler.c
	$(CC) assembler.c -o assembler


clean:
	rm -f main
