# Elijah Greig
# 3128908
# Submitted on: Mar 22 2026
# File: Makefile | Makefile for vmsim application

GCC= gcc
CFLAGS= -std=c11 -Wall -Wextra -pedantic
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

.PHONY: userclean clean valgrind

all: vmsim.o parse_seg.o parse_bb.o
	$(GCC) $(CFLAGS) -o vmsim $^ 

%.o: %.c
	$(GCC) -c $<

valgrind: 
	valgrind $(VFLAGS) ./vmsim --mode=seg --config=tests/seg/three-seg.ini --trace=tests/seg/t2.txt
	valgrind $(VFLAGS) ./vmsim --mode=bb --base=1 --limit=100 --trace=tests/bb/t1.txt

test: 
	@echo "Testing base/bounds on tests/bb/t1.txt, base 4096, limit 64"
	@./check_output.sh "./vmsim --mode=bb --base=4096 --limit=64 --trace=tests/bb/t1.txt" tests/bb/test_t1.txt

	@echo "\nTesting bad base/bounds on tests/bb/bad.txt, base 4096, limit 64"
	@./check_output.sh "./vmsim --mode=bb --base=4096 --limit=64 --trace=tests/bb/bad.txt" tests/bb/test_bad.txt

	@echo "\nTesting seg on tests/seg/t2.txt with config tests/seg/three-seg.ini"
	@./check_output.sh "./vmsim --mode=seg --config=tests/seg/three-seg.ini --trace=tests/seg/t2.txt" tests/seg/test_t2.txt

	@echo "\nTesting seg on tests/seg/t2.txt with bad config tests/seg/bad-config.ini"
	@./check_output.sh "./vmsim --mode=seg --config=tests/seg/bad-config.ini --trace=tests/seg/t2.txt" tests/seg/test_bad.txt


clean: 
	rm -f *.o vmsim