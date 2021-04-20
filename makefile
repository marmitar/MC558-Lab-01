CC = gcc

OPTIM   = -O3 -march=native -mtune=native -pipe -fivopts
OPTIM  += -fmodulo-sched -flto -fwhole-program -fno-plt
CFLAGS ?= -std=c11 -Wall -Werror -Wpedantic -Wunused-result $(OPTIM)


t1: t1.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f t1

debug: OPTIM = -O0 -ggdb3 -DDEBUG
debug: clean t1

.PHONY: clean debug
