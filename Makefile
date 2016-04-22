
AR ?= ar
CC ?= gcc
PREFIX ?= /usr/local

CFLAGS = -O3 -std=c99 -Wall -Wextra -Ideps

SRCS = src/cache_control.c \

OBJS = $(SRCS:.c=.o)

all: build/libcachecontrol.a

install: all
	cp -f build/libcachecontrol.a $(PREFIX)/lib/libcachecontrol.a
	cp -f src/cache_control.h $(PREFIX)/include/cache_control.h

uninstall:
	rm -f $(PREFIX)/lib/libcachecontrol.a
	rm -f $(PREFIX)/include/cache_control.h

build/libcachecontrol.a: $(OBJS)
	@mkdir -p build
	$(AR) rcs $@ $^

bin/test: test.o $(OBJS)
	@mkdir -p bin
	$(CC) $^ -o $@

bin/benchmark: benchmark.o $(OBJS)
	@mkdir -p bin
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -fr bin build *.o src/*.o

test: bin/test
	@./$<

benchmark: bin/benchmark
	@./$<

.PHONY: test benchmark clean install uninstall
