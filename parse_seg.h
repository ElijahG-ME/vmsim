// Elijah Greig
// 3128908
// Submitted on: Mar 22 2026
// File: parse_seg.h | Header file for vmsim application

#ifndef PARSE_SEG_H
#define PARSE_SEG_H
#include "vmsim.h"

void parse_config(char* config, struct segment* segments);
void parse_trace(char* trace, struct access* accesses, struct segment* segments);
void permissions_print(int op, struct segment* seg);


#endif