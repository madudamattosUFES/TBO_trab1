CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

SRCS = fila.c grafo.c heap.c main_teste_heap.c
OBJS = $(SRCS:.c=.o)
EXEC = main_teste_heap

all: $(EXEC)

$(EXEC): $(OBJS)
    $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(EXEC)

.PHONY: all clean