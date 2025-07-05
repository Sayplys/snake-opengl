CC = g++
CFLAGS = -Wall
LDFLAGS = -lglut -lGL -lGLU

SRCS = main.c game.c snake.c utils.c
BIN_DIR = bin
OBJS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/snake_game

all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BIN_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)