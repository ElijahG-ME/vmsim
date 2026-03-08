#ifndef VMSIM_H
#define VMSIM_H


enum operation{
    READ,
    WRITE,
    EXECUTE
};
enum result{
    OK,
    BOUNDS,
    PROTECTION,
    NOSEG
};

enum seg{
    CODE,
    HEAP,
    STACK
};

struct access {
    enum operation o;
    int address;
    enum seg seg;
    enum result result;
};
#endif