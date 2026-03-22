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

## Functionality<br>

VMsim works in one of two modes with the proper arguments.<br>
### Base & Bounds (BB):<br>
       - The base and limit are recorded<br>
       - The trace file is evaluated line by line, reading the access instructions<br>
       - If malformed or invalid, an error message is printed<br>
       - If valid, the successful operation will be printed<br>
       - This repeats until EOF<br>
       - Statistics about the operations including counts for successful accesses & faults are printed<br>

### Segmentation (Seg):<br>
       - The config file is read line by line<br>
       - Objects for each segment are created<br>
       - Information about each segment is recorded to its corresponding object, if applicable<br>
       - Errors in file formatting are printed
       - The trace file is read line by line<br>
       - Access operations on each segment are performed if valid<br>
       - If malformed or invalid, an error message is printed<br>
       - Faults are recorded to the access if they occur, specified by type
       - Statistics about the operations including counts for successful accesses & faults and fault types are printed<br>
