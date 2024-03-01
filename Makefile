CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2 -llua

SRCS = main.c
OBJS = $(SRCS:.c=.o)
	EXEC = game_engine

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	    $(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	    $(CC) $(CFLAGS) -c $< -o $@

clean:
	    rm -f $(EXEC) $(OBJS)

