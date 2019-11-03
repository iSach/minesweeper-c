SRCS = minesweeper.c Engine.c Grid.c Solver.c
OBJS = $(SRCS:%.c=%.o)
CC = gcc
PROG = minesweeper


CFLAGS = -Wall -Wextra -Weverything -Wmissing-prototypes --pedantic -std=c99
LDFLAGS = -lm

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.PHONY: clean run

clean:
	$(RM) $(OBJS) $(PROG)

run: $(PROG)
	./$(PROG) 30 16 80 1

runwin: $(PROG)
	$(PROG).exe
