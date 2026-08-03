TARGET      = bin/chipdale
TEST_TARGET = bin/chipdale_tests

CC       = clang
CFLAGS   = -std=c17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -O2
CPPFLAGS = -Isrc

SRC     = $(wildcard src/*.c)
OBJ     = $(SRC:src/%.c=bin/%.o)
LIB_OBJ = $(filter-out bin/main.o,$(OBJ))

TEST_SRC    = $(wildcard tests/*.c)
TEST_OBJ    = $(TEST_SRC:tests/%.c=bin/tests/%.o)
TEST_LDLIBS = -lcriterion

DEPS = $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)

all: $(TARGET)

options:
	@echo "TARGET      = $(TARGET)"
	@echo "TEST_TARGET = $(TEST_TARGET)"
	@echo
	@echo "CC       = $(CC)"
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "CPPFLAGS = $(CPPFLAGS)"
	@echo
	@echo "SRC     = $(SRC)"
	@echo "OBJ     = $(OBJ)"
	@echo "LIB_OBJ = $(LIB_OBJ)"
	@echo
	@echo "TEST_SRC    = $(TEST_SRC)"
	@echo "TEST_OBJ    = $(TEST_OBJ)"
	@echo "TEST_LDLIBS = $(TEST_LDLIBS)"

bin/:
	mkdir -p $@

bin/tests/:
	mkdir -p $@

bin/%.o: src/%.c | bin/
	$(CC) -o $@ $< -c -MMD -MP $(CPPFLAGS) $(CFLAGS)

bin/tests/%.o: tests/%.c | bin/tests/
	$(CC) -o $@ $< -c -MMD -MP $(CPPFLAGS) $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

format:
	clang-format -i src/*.[ch] tests/*.c

format-check:
	clang-format --dry-run --Werror src/*.[ch] tests/*.c

lint:
	clang-tidy $(SRC) $(TEST_SRC) -- $(CPPFLAGS) $(CFLAGS)

$(TEST_TARGET): $(LIB_OBJ) $(TEST_OBJ)
	$(CC) -o $@ $^ $(TEST_LDLIBS)

test: $(TEST_TARGET)
	./$<

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJ) $(TEST_OBJ) $(DEPS)

-include $(DEPS)

.PHONY: all options format format-check lint test clean
