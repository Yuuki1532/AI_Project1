SRC = ./src
OBJ = ./obj
BIN = ./bin
INCLUDE = ./include

TARGET = $(BIN)/main
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

CC = g++
CFLAGS = -std=c++14 -Wall -O3

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -I$(INCLUDE) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
