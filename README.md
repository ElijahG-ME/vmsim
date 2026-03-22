**Elijah Greig**
**3128908**
**Submitted on: Mar 22 2026**
**File: README**

Academic integrity statement:
*"I certify that this submission represents entirely my own work."*

## To compile and run
Write `make` into the shell for the executable binary to be created.
Usage: ./vmsim \n --mode=bb  --base=N --limit=N --trace=FILE  *OR*
       ./vmsim --mode=seg --config=FILE --trace=FILE 

Tests: Tests can be run with `make test` and it will compare program output to the expected output. 
    - A shell script included in this folder by the name of `check_output.sh` will use diff to compare the output to provided expected output files
    - These files are test_t1.txt and test_t2.txt in the tests folder

Memory checking: Memory leak checks can be performed with `make valgrind'.

## Program Status

The program currently works as intended, with no known deficiencies or errors.
