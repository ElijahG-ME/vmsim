**Elijah Greig**<br>
**3128908**<br>
**Submitted on: Mar 22 2026**<br>
**File: README**<br>

Academic integrity statement:<br>
*"I certify that this submission represents entirely my own work."*<br>

## To compile and run<br>
Write `make` into the shell for the executable binary to be created.<br>
Usage: ./vmsim \n --mode=bb  --base=N --limit=N --trace=FILE  *OR*<br>
       ./vmsim --mode=seg --config=FILE --trace=FILE <br>

Tests: Tests can be run with `make test` and it will compare program output to the expected output. <br>
    - A shell script included in this folder by the name of `check_output.sh` will use diff to compare the output to provided expected output files<br>
    - These files are test_t1.txt and test_t2.txt in the tests folder<br>

Memory checking: Memory leak checks can be performed with `make valgrind'.<br>

## Program Status<br>

The program currently works as intended, with no known deficiencies or errors.<br>
