CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = mysh
SRCS = main.c parser.c executor.c jobs.c signals.c builtins.c strdup.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean