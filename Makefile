CC = gcc
CFLAGS = -Wall -Wextra -g
TARGETS = semaphore pipes fork thread

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(TARGETS)
