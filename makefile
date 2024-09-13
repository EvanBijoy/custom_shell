CC = gcc

INCLUDE_DIR = include
SRC_DIR = source

CFLAGS = -w -I$(INCLUDE_DIR)

SRCS = $(wildcard $(SRC_DIR)/*.c)

EXEC = shell

all: clean $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EXEC)

clean:
	-rm -f $(EXEC)

.PHONY: all clean
