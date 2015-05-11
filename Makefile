all: main

CFLAGS += -g -std=c99

main: main.c vm.h vm.c
	$(CC) $(CFLAGS) main.c vm.c -o main


clean:
	rm -f main
