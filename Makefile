CC = g++
SRCS = ./src/*.cpp
INC = ./src/
OPTS = -std=c++17 -Wall -Werror -lpthread -O3

EXEC = bin/prefix_scan

all: clean compile
debug: clean compile_debug

compile_debug:
	$(CC) $(SRCS) $(OPTS) -g -I$(INC) -o $(EXEC)

compile:
	$(CC) $(SRCS) $(OPTS) -I$(INC) -o $(EXEC)

clean:
	rm -f $(EXEC)
