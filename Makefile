CC = gcc
EDCFLAGS := -Wall -O2 -std=gnu11 -I ./ -I ./Si446x $(CFLAGS)
EDLDFLAGS := -lpthread $(LDFLAGS)

C_SRCS = $(wildcard *.c) \
		$(wildcard Si446x/*.c) \
		gpiodev/gpiodev.c \
		spibus/spibus.c

COBJS = $(C_SRCS:.c=.o)

all: $(COBJS)
	$(CC) -o tx.out $(COBJS) $(EDLDFLAGS)
	
%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -vf $(COBJS)
	rm -vf *.out