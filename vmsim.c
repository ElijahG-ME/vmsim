// Elijah Greig
// 3128908
// Submitted on: Mar 22 2026
// File: vmsim.c | Source file for vmsim application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vmsim.h"
#include "parse_bb.h"
#include "parse_seg.h"

void usage(){ 
    printf("Usage: ./vmsim \n --mode=bb  --base=N --limit=N --trace=FILE \n --mode=seg --config=FILE --trace=FILE \n");
}

int is_number(char* num){
    // checks if string is composed only of digits
    // accounts for whitespace

    for (int j = 0; num[j] != '\0'; j++) {
        if (!isdigit(num[j]) && num[j] != '\r') { // carriage returns are known to cause problems
            return 0;
        }
    }
    return 1;
}

void bb_stats(struct access* a, int count){
    // evaluates and prints stats for base/bounds operation

    printf("== stats ==\n");
    int accesses = 0; int ok = 0; int faults = 0;
    for (int i = 0; i < count; i++){
        if (!a[i].dirty){
            accesses++;
            switch(a[i].result){
                case 0: ok++; break;
                case 1: faults++; break;
            }
            
        }
       
    }
    printf("accesses=%d, ok=%d, faults.bounds=%d\n", accesses, ok, faults);

}

void seg_stats(struct access* a, int count){
    // evaluates and prints states for segmentation operation

    printf("== stats ==\n");
    int accesses = 0; int ok = 0; int bounds = 0; int protection = 0; int noseg = 0;
    int c = 0; int h = 0; int s = 0;
    
    for (int i = 0; i < count; i++){
        
        if (!a[i].dirty){
            accesses++;
        
            switch (a[i].result){
                case OK: ok++; break;
                case BOUNDS: bounds++; break;
                case PROTECTION: protection++; break;
                case NOSEG: noseg++; break;
            } 
            switch (a[i].seg){
                case CODE: if (a[i].result == OK) {c++;} break;
                case HEAP: if (a[i].result == OK) {h++;} break;
                case STACK: if (a[i].result == OK) {s++;} break;
            }
        }
        
    }
    printf("accesses=%d, ok=%d, faults.bounds=%d\nfaults.prot=%d, faults.noseg=%d\nseg_hits: code=%d, heap=%d, stack=%d\n", accesses, ok, bounds, protection, noseg, c, h, s);

}

void bb(int base, int bounds, char* filename){
    // Runs base/bounds functions 

    int count = count_accesses(filename);
    if (count == -1){
        return;
    }

    struct access accesses[count];
    struct access *a = accesses;
    
    run_accesses_bb(base, bounds, filename, a); // main operations
    bb_stats(a, count); // stats printing

    return;
    
}

void seg(char* config, char* trace){
    // run segmentation functions

    int count = count_accesses(trace);
    if (count == -1){
        return;
    }

    struct access accesses[count];
    struct access *a = accesses;
    struct segment segments[3]; // fixed positions: 0 for code, 1 for heap, 2 for stack

    for (int i = 0; i < 3; i++){ // all values initialized to false
        segments[i].declared = 0;
        segments[i].r = 0;
        segments[i].w = 0;
        segments[i].x = 0;
    }
    
    parse_config(config, segments); // parses config file
    parse_trace(trace, accesses, segments); // parses trace file and evaluates accesses
    seg_stats(a, count); // stats printing

    return;
    
}

int main(int argc, char **argv){
    // parse CLI
    // Check if number of args is valid

    if (argc > 5 || argc < 4){
        usage();
        return 1;
    }

    int mode = 0; // Mode is 1 for bb, 2 for seg
    // evaluating the first arg

    if (strcmp(argv[1], "--mode=bb") == 0){ 
        mode = 1;
        if (argc != 5) {usage(); return 1;}
    }
    else if (strcmp(argv[1], "--mode=seg") == 0){
        mode = 2;
        if (argc != 4) {usage(); return 1;}
    }
    else if (!mode){
        printf("Mode provided was not valid.\n");
        usage();
        return 1;
    }

    switch (mode){

        // bb mode
        case 1: {
            long base;
            long limit;
            char* filename;

            // ensure next argument is base
            if (strncmp(argv[2], "--base=", 7) == 0){ // comparing first 7 characters of the arg string for correct usage  

                // ensuring number isn't negative
                if ((*(argv[2]+7) == '-')) {usage(); return 1;}

                char* end;
                base = strtol(argv[2]+7, &end, 10); // strtol returns the numbers within the string as a long
                
                if ((argv[2]+7) == end) { // "end" will equal the original string if the operation failed (invalid input)
                    printf("Base provided was not a valid number.\n");
                    usage(); return 1;
                }
            }


            // ensure next argument is limit
            if (strncmp(argv[3], "--limit=", 8) == 0){ // comparing first 7 characters of the arg string for correct usage   
                
                // ensuring number isn't negative
                if ((*(argv[2]+7) == '-')) {usage(); return 1;}

                char* end;
                limit = strtol(argv[3]+8, &end, 10);

                if ((argv[3] + 8) == end) { // "end" will equal the original string if the operation failed (invalid input)
                    printf("Limit provided was not a valid number.\n");
                    usage(); return 1;
                }
            }
            else { usage(); return 1; }

            // ensure next argument is trace
            if (strncmp(argv[4], "--trace=", 8) == 0){ // comparing first 5 characters of the arg string for correct usage
                filename = malloc(strlen(argv[4]+8) + 1); // moves pointer to count length of string past equals sign
                if (filename == NULL) {
                        printf("Memory allocation failed");
                        return 1;
                    } 
                strcpy(filename, argv[4] + 8);
                
            }
            else { usage(); return 1; }
            
            
           bb(base, limit, filename);
           free(filename);

        }
            
        break;



        // seg mode
        case 2: {
            char* filename_config;
            char* filename_trace;

            // ensure next argument is config
            if (strncmp(argv[2], "--config=", 9) == 0){ // comparing first 5 characters of the arg string for correct usage
                filename_config = malloc(strlen(argv[2] + 9) + 1); // moves pointer to count length of string past equals sign
                if (filename_config == NULL) {
                        printf("Memory allocation failed");
                        return 1;
                    } 
                strcpy(filename_config, argv[2] + 9);
                
            }
            else { usage(); return 1; }
            
            // ensure next argument is trace
            if (strncmp(argv[3], "--trace=", 8) == 0){ // comparing first 5 characters of the arg string for correct usage
                filename_trace = malloc(strlen(argv[3] + 8) + 1); // moves pointer to count length of string past equals sign
                if (filename_trace == NULL) {
                        printf("Memory allocation failed");
                        return 1;
                    } 
                strcpy(filename_trace, argv[3] + 8);
                
            }
            else { usage(); return 1; }

            seg(filename_config, filename_trace);
            free(filename_trace); free(filename_config);
        }
        
        break;

        // Should never be reached: 
        default:
            usage();
            return 1;
        break;
    }

    return 0;

}