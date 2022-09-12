CC = g++
SRCS = ./src/*.cpp
INC = ./src/
OPTS = -std=c++17 -Wall -Werror -lpthread

EXEC = bin/prefix_scan
EXEC_DEBUG = bin/prefix_scan_debug

all: clean compile test
debug: compile_debug

compile_debug:
	$(CC) $(SRCS) $(OPTS) -g -I$(INC) -o $(EXEC_DEBUG)

compile:
	$(CC) $(SRCS) $(OPTS) -O3 -I$(INC) -o $(EXEC)

clean:
	rm -f $(EXEC)

test:
	python run_tests.py