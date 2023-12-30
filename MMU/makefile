
CC = gcc
CCOPTS = --std=gnu99 -Wall
AR = ar

OBJS= main.o mmu_fake.o
HEADERS= define.h
BINS= run_program 


.PHONY: clean all
all: $(LIBS) $(BINS)
%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@ $<


run_program: $(OBJS)
	$(CC) $(CCOPTS) -o  $@ $^

clean: rm -rf *.o *~ $(LIBS) $(BINS)
