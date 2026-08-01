TARGET = bin/chipdale

CC     = clang
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=bin/%.o)

all: $(TARGET)

options:
	@echo "TARGET = $(TARGET)"
	@echo "CC     = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "SRC    = $(SRC)"
	@echo "OBJ    = $(OBJ)"

bin/:
	mkdir -p $@

bin/%.o: src/%.c | bin/
	$(CC) -o $@ $< -c $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

format:
	clang-format -i src/*.[ch]

lint:
	clang-tidy $(SRC) -- $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all options format lint clean
