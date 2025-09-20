CXX      := g++

CXXFLAGS := -Wall -g -std=c++11

CC       := gcc

CFLAGS   := -Wall -g

INCFLAGS := -Iinclude \
            -Ilib/log \
            -Ilib/dequeue \
			-Ilib/windowContext

LDFLAGS  := -lSDL2 -lSDL2_ttf -lpthread

CPPSRC   := osc.cpp

CSRC     := $(wildcard lib/log/*.c) \
            $(wildcard lib/dequeue/*.c) \
            $(wildcard lib/windowContext/*.c)

OBJ      := $(CPPSRC:.cpp=.o) $(CSRC:.c=.o)

BIN      := osc

.PHONY: all sim clean deps exec leak_check

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

exec: $(BIN)
	./$(BIN)

clean:
	rm -vf $(OBJ) $(BIN)

leak_check:
	valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all \
        --log-file=valgrind.%p.log ./$(BIN)
