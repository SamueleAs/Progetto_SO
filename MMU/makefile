CC = gcc
CCOPTS=  --std=gnu99 -Wall
AR= ar

OBJS= main.o mmu_fake.o
HEADERS= define.h
BINS= exec



%.o: %.c $(HEADERS) $(CC) $(CCOPTS) -c -o $@ $<
.phony: clean all
all: $(BINS) $(LIBS)

exec: $(OBJS) $(CC) $(CCOPTS) -o -c $@ $^
CLEAN: rm -rf *.o *.~ $(LIBS) $(BINS)
