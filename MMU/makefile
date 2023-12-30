
CC = gcc
CCOPTS = --std=gnu99 -Wall
AR = ar

OBJS= main.o mmu_fake.o
HEADERS= define.h
BINS= exec 


.PHONY: clean all
all: $(LIBS) $(BINS)
%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<


exec: $(OBJS)
	$(CC) $(CCOPTS) -o  $@ $^

clean: rm -rf *.o *~ $(LIBS) $(BINS)
