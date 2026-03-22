// Elijah Greig
// 3128908
// Submitted on: Mar 22 2026
// File: parse_seg.c | Source file for vmsim application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vmsim.h"
#include "parse_seg.h"

void parse_config(char* config, struct segment* segments){
    // parsing config file to create segments
    
    // pointers to three segments are made (always will remain in same position of array)
    struct segment* code = &segments[0];
    struct segment* heap = &segments[1];
    struct segment* stack = &segments[2];

    // Config Parsing

    FILE* config_file = fopen(config, "r");

    if (config_file){

        int i = 1;
        char ch = fgetc(config_file);
        int commented_lines = 0;
        while (ch != EOF){
            
            // if line begins with "#", skip to end of line
            if (ch == '#'){ 
                commented_lines++; // count commented lines for later offset when printing
                while (ch != '\n') {
                    ch = fgetc(config_file);
                    if (ch == EOF) {return;}
                }
                ch = fgetc(config_file);
                i++;
                continue; 
            }
            
            // On line to be read: write line to string
            int line_size = 30; char* line = malloc(line_size); // Starts at size 30. Will be increased if needed
            if (!line) { 
                printf("Allocation failed\n"); 
                return;
            }
            int index = 0;

            while (ch != '\n' && ch != -1) {
                // check if we're full, reallocate to double previous length if so
                if (index == line_size) {
                    line_size *= 2;
                    line = realloc(line, line_size); 
                    if (!line) { printf("Allocation failed\n"); return;}
                }
                line[index] = ch;
                index++;
                ch = fgetc(config_file);

            }
            // Now at end of line
            line[index] = '\0'; // add null terminator

            char* segment = strtok(line, " ");
            if (!segment) { 
                printf("config: %s:%d bad segment name (expected code, heap, stack)\n", config, i + commented_lines);
                free(line);
                ch = fgetc(config_file); i++;
                continue;

            }

            if (strcmp(segment, "code") == 0) {
                char* base_c = strtok(NULL, " ");
                if (!base_c || !is_number(base_c)){ // evaluate validity of segment base
                    printf("config: %s:%d bad number in base\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }
                code->base = atoi(base_c);

                char* limit_c = strtok(NULL, " "); 
                if (!limit_c || !is_number(limit_c)){ // evaluate validity of segment limit
                    printf("config: %s:%d bad number in limit\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }
                code->limit = atoi(limit_c);
                
                char* perms = strtok(NULL, " "); // take perms from line
                
                if (strchr(perms, 'r')) {code->r = 1;}
                if (strchr(perms, 'w')) {code->w = 1;}
                if (strchr(perms, 'x')) {code->x = 1;}

                if (!code->r &!code->w & !code->x){ // if no perms were recorded
                    printf("config: %s:%d malformed: expected 'name base limit perms'\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }

                code->declared = 1; // segment is declared
            }

            // === Next two statements follow same logic as above ===

            if (strcmp(segment, "heap") == 0) {
                char* base_c = strtok(NULL, " ");
                if (!base_c || !is_number(base_c)){
                    printf("config: %s:%d bad number in base\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }
                heap->base = atoi(base_c);

                char* limit_c = strtok(NULL, " ");
                if (!limit_c || !is_number(limit_c)){
                    printf("config: %s:%d bad number in limit\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }
                heap->limit = atoi(limit_c);
                
                char* perms = strtok(NULL, " ");
                if (strchr(perms, 'r')) {heap->r = 1;}
                if (strchr(perms, 'w')) {heap->w = 1;}
                if (strchr(perms, 'x')) {heap->x = 1;}
                if (!heap->r &!heap->w & !heap->x){
                    printf("config: %s:%d malformed: expected 'name base limit perms'\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }

                heap->declared = 1;
            }

            if (strcmp(segment, "stack") == 0) {
                char* base_c = strtok(NULL, " ");
                if (!base_c || !is_number(base_c)){
                    printf("config: %s:%d bad number in base\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }
                stack->base = atoi(base_c);

                char* limit_c = strtok(NULL, " ");
                if (!limit_c || !is_number(limit_c)){
                    printf("config: %s:%d bad number in limit\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }
                stack->limit = atoi(limit_c);
                
                char* perms = strtok(NULL, " ");
                if (strchr(perms, 'r')) {stack->r = 1;}
                if (strchr(perms, 'w')) {stack->w = 1;}
                if (strchr(perms, 'x')) {stack->x = 1;}
                if (!stack->r &!stack->w & !stack->x){
                    printf("config: %s:%d malformed: expected 'name base limit perms'\n", config, i + commented_lines);
                    free(line);
                    ch = fgetc(config_file); i++;
                    continue;
                }

                stack->declared = 1;
            }
            // ===  ===

            free(line);
            ch = fgetc(config_file); i++;
            continue;
        }
    }
    fclose(config_file);
}

void permissions_print(int op, struct segment* seg){
    // helper function for printing permissions fault details

    char operation_letter; // access operation is marked as a char for printing
    switch (op){
        case READ:
            operation_letter = 'r';
            break;
        case WRITE:
            operation_letter = 'w';
            break;
        case EXECUTE:
            operation_letter = 'x';
            break;
    }


    int permissions = ((seg->r) * 100) + ((seg->w) * 10) + ((seg->x)); 
    // Permissions is turned into a 3 digit "binary" number for case writing
    // Example: 101 means read & execute, 000, means none, 110 means read & write
    // Used for printing protection fault for a specific segment access

    
    switch (permissions){
        case 000:
            printf("(needed '%c', have ' ')", operation_letter);
            break;
        case 001:
            printf("(needed '%c', have 'x')", operation_letter);
            break;
        case 010:
            printf("(needed '%c', have 'w')", operation_letter);
            break;
        case 011:
            printf("(needed '%c', have 'wx')", operation_letter);
            break;
        case 100:
            printf("(needed '%c', have 'r')", operation_letter);
            break;
        case 101:
            printf("(needed '%c', have 'rx')", operation_letter);
            break;
        case 110:
            printf("(needed '%c', have 'rw')", operation_letter);
            break;
}


}

void parse_trace(char* trace, struct access* accesses, struct segment* segments){
    // Trace file parsing

    struct segment* code = &segments[0];
    struct segment* heap = &segments[1];
    struct segment* stack = &segments[2];

    FILE* trace_file = fopen(trace, "r");

    if (trace_file){

        int i = 0;
        char ch = fgetc(trace_file);
        int commented_lines = 0;
        while (ch != EOF){
            
            
            // if line begins with "#", skip to end of line
            if (ch == '#'){ 
                // but keep track of this for an offset
                commented_lines++;
                
                while (ch != '\n') {
                    ch = fgetc(trace_file);
                    if (ch == EOF) {return;}
                }
                ch = fgetc(trace_file);
                continue; 
            }
            
            // On line to be read: write line to string
            int line_size = 30; char* line = malloc(line_size); // Starts at size 30. Will be increased if needed
            if (!line) { 
                printf("Allocation failed\n"); 
                return;
            }
            int index = 0;

            while (ch != '\n' && ch != -1) {
                // check if we're full, reallocate to double previous length if so
                if (index == line_size) {
                    line_size *= 2;
                    line = realloc(line, line_size); 
                    if (!line) { printf("Allocation failed\n"); return;}
                }
                line[index] = ch;
                index++;
                ch = fgetc(trace_file);

            }
            // Now at end of line
            line[index] = '\0'; // add null terminator

            char whole_line[strlen(line)+1];
            strcpy(whole_line, line);       // Whole line saved for easier printing later

            char* op = strtok(line, " ");

            accesses[i].dirty = 0; // access is marked as dirty if the line is malformed

            if(strchr(op, 'R')) { accesses[i].o = READ; op = "R";}
            else if(strchr(op, 'W')) { accesses[i].o = WRITE; op = "W";}
            else if(strchr(op, 'X')) { accesses[i].o = EXECUTE; op = "X";}
            else { // If no valid operation was written
                printf("trace: %s:%d, malformed: op must be R/W/X, got %s\n", trace, i + commented_lines + 1, op);
                accesses[i].dirty = 1;
                free(line);
                ch = fgetc(trace_file); i++;
                continue;
            }

            char* seg = strtok(NULL, " "); // segment recorded and evaluated
            if (strcmp(seg, "code") == 0) {
                accesses[i].seg = CODE;
            } 
            else if (strcmp(seg, "heap") == 0) {
                accesses[i].seg = HEAP;
            } 
            else if (strcmp(seg, "stack") == 0) {
                accesses[i].seg = STACK;
            } 
            else {
                printf("trace: %s:%d malformed: expected 'OP SEG OFFSET' (non-negative raw offset)\n", trace, i + commented_lines + 1);
                accesses[i].dirty = 1;
            }
            char* off; 

            if (!accesses[i].dirty){
                off = strtok(NULL, " ");
                if (strchr(off, '-')) { // Negative number
                    printf("%s\t-> trace: %s:%d malformed: expected 'OP SEG OFFSET' (non-negative raw offset)\n", whole_line, trace, i + commented_lines + 1);
                    accesses[i].dirty = 1;
                 }

                else if (!off || !is_number(off)) { // Malformed input (letters or symbols in number, or absence of entry)
                    printf("trace: %s:%d malformed: expected 'OP SEG OFFSET' (non-negative raw offset)\n", trace, i + commented_lines + 1);
                    accesses[i].dirty = 1;
                }
            }
            
            
            // Printing output

            if (accesses[i].dirty) { // if dirty, print has already been performed
                free(line);
                ch = fgetc(trace_file); i++;
                continue;
            }
            else {
                accesses[i].address = atoi(off); // offset is saved to address
                // assume OK until issue
                accesses[i].result = OK;

                int stack_calc = 0; // bool for if we are doing a stack calculation
                int PA;
                struct segment* curr; // segment that the current access is targeting

                switch(accesses[i].seg){
                    case CODE:
                        {curr = code;}
                    break;

                    case HEAP:
                        {curr = heap;}
                    break;

                    case STACK:{
                        curr = stack;
                        // special calculation for stack growth
                        stack_calc++;
                        int off_signed = accesses[i].address - curr->limit;
                        if (abs(off_signed) > curr->limit || off_signed < (-1 * curr->limit) || off_signed > (-1) || accesses[i].address < 0){
                            accesses[i].result = BOUNDS;
                        }
                        else {
                            PA = off_signed + curr->base;
                        }
                        
                    }
                    break;
                }
                                
                if (!(curr->declared)) { // if targeted segment was not declared in config
                    accesses[i].result = NOSEG;
                }
                else if ((accesses[i].address < 0 || accesses[i].address > code->limit) && !stack_calc){
                    accesses[i].result = BOUNDS;
                }

                else {

                    switch(accesses[i].o){ // Evaluate operation
                        case READ:
                            if (!curr->r){ accesses[i].result = PROTECTION; }
                        break;

                        case WRITE:
                            if (!curr->w){ accesses[i].result = PROTECTION; }
                        break;
                        
                        case EXECUTE:
                            if (!curr->x){ accesses[i].result = PROTECTION; }
                        break;
                    }
                    
                }
                switch(accesses[i].result){
                    case OK:
                        if (!stack_calc) { PA = accesses[i].address + curr->base; } // PA will have been previously calculated for stack segment operations
                        printf("%s\t-> PA %d ; ok\n", whole_line, PA);
                    break;

                    case BOUNDS:
                        printf("%s\t-> fault: BOUNDS\n", whole_line);
                    break;

                    case PROTECTION:
                        printf("%s\t-> fault: PROTECTION ", whole_line);
                        permissions_print(accesses[i].o, curr);
                        printf("\n");
                    break;

                    case NOSEG:
                        printf("%s\t-> fault: NOSEG\n", whole_line);
                    break;

                }
                
                free(line);
                ch = fgetc(trace_file); i++;    
        
            }
        }

    }
    fclose(trace_file);
   
}


