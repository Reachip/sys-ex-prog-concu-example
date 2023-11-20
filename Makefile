CC = gcc
CFLAGS = -Wall -Wextra -g
TARGETS = semaphore pipes

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(TARGETS)
