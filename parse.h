// Elijah Greig
// 3128908
// Submitted on: 
// File: parse.h | Header file for vmsim application

#ifndef PARSE_H
#define PARSE_H
#include "vmsim.h"

int count_accesses(char* filename);
int run_accesses(int base, int bounds, char* filename, int mode, struct access* accesses);
#endif