CC = gcc
CCOPTS = --std=gnu99 -Wall
AR = ar

OBJS= main.o mmu.o
HEADERS= define.h
BINS= exec


.phony: clean all
all: $(BINS) $(LIBS)
%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<


exec: $(OBJS)
	$(CC) $(CCOPTS) -o  $@ $^
clean: rm -rf *.o *~ $(LIBS) $(BINS)
