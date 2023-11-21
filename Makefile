CC = gcc
CFLAGS = -Wall -Wextra -g
TARGETS = semaphore pipes fork rdv share_mem tour_boucle pipe_av calculateur mutex_thread

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(TARGETS)
