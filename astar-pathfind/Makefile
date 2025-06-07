CFLAGS = -O2 -Wall -Werror -Wpedantic
SRCS = astar.c minheap.c
run: $(SRCS)
	gcc $(CFLAGS) $(SRCS) -o astar.out
	./astar.out

.PHONY: clean

clean: astar.out
	rm -f astar.out
