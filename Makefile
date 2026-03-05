# Elijah Greig
# 3128908
# Submitted on: 
# File: Makefile | Makefile for vmsim application

GCC= gcc
CFLAGS= -std=c11 -Wall -Wextra -pedantic
VFLAGS = --leak-check=yes

.PHONY: userclean clean valgrind

all: vmsim.o 
	$(GCC) $(CFLAGS) -o vmsim $^ 

%.o: %.c
	$(GCC) -c $<

valgrind: 
	valgrind $(VFLAGS) ./vmsim

clean: 
	rm -f *.o vmsim