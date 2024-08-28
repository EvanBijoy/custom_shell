# Compiler
CC = gcc

# Directories
INCLUDE_DIR = include
SRC_DIR = source

# Compiler flags
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Executable name
EXEC = shell

# Target rules
all: $(EXEC)

$(EXEC): clean
	$(CC) $(CFLAGS) $(SRCS) -o $(EXEC)

clean:
	rm -f $(EXEC)

.PHONY: all clean
