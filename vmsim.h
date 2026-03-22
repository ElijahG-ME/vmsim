// Elijah Greig
// 3128908
// Submitted on: Mar 22 2026
// File: vmsim.h | Header file for vmsim application
#ifndef VMSIM_H
#define VMSIM_H


enum operation{
    READ, // 0
    WRITE, // 1
    EXECUTE // 2
};
enum result{ // result of memory access
    OK,
    BOUNDS,
    PROTECTION,
    NOSEG
};

enum seg{
    CODE, // 0
    HEAP, // 1
    STACK // 2
};


struct access {
    enum operation o; // operation of access (rwx)
    int address; // virtual address of access
    enum seg seg; // corresponding seg of access (if applicable)
    enum result result; // result of access
    int dirty; // true if access instructions are malformed 
};

struct segment {
    int base;
    int limit;
    int r; int x; int w;
    int declared;
};

int is_number(char* num);
int count_accesses(char* filename);

#endif