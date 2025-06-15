build:
	gcc main.c src/block/block.c src/list/list.c src/heap/heap.c -Wall -Wextra -std=c99 -lm -g -o sfl

clean:
	rm -rf sfl

run_sfl:
	./sfl 2> /dev/null