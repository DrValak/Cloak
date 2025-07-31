CC = gcc
CFLAGS = -Wall -g

SRC = src/main.c src/ring_buffer.c src/crypt.c src/network.c
OBJ = $(SRC:.c=.o)
EXEC = cloak

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

