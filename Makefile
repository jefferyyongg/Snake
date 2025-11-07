CC = gcc
CFLAGS = -std=c11 -Wextra -Wpedantic -g3 -fsanitize=address

# Detect OS and set appropriate flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS
    LDFLAGS = -lncurses -fsanitize=address
    CFLAGS += -I/opt/homebrew/opt/ncurses/include -I/usr/local/opt/ncurses/include
else
    # Linux
    LDFLAGS = -lncursesw -fsanitize=address
endif

SRC = snake.c
HEADERS = $(wildcard *.h)

.PHONY: all clean run setup tarball check-deps

all: spel

check-deps:
	@command -v gcc >/dev/null 2>&1 || command -v clang >/dev/null 2>&1 || { echo "ERROR: No C compiler found"; exit 1; }
	@echo "Dependencies OK ✓"

spel: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

setup:
	@echo "Checking locale support..."
	@if ! locale -a | grep -q "en_US.utf8\|en_US.UTF-8"; then \
		echo "WARNING: UTF-8 locale not found. Emojis may not display correctly."; \
	else \
		echo "UTF-8 locale found ✓"; \
	fi

run: spel
	@export LANG=en_US.UTF-8; \
	export LC_ALL=en_US.UTF-8; \
	export LC_CTYPE=en_US.UTF-8; \
	./spel

tarball: spel.tar.gz

spel.tar.gz: $(SRC) $(HEADERS) Makefile README.md
	tar czf $@ $^

clean:
	rm -f *~ *.o spel snake.tar.gz