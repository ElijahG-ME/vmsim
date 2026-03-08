// Elijah Greig
// 3128908
// Submitted on: 
// File: parse.c | Source file for vmsim application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vmsim.h"


int count_accesses(char* filename){// Count lines in file
    // Arguments: filename
    FILE* file;
    int count = 0;
    int last_char = '\n';
    if (file = fopen(filename, "r")){

        char ch = fgetc(file);
        while (ch != EOF){
            if (last_char == '\n') {
                if (ch != '#') {count += 1;}
            }
            last_char = ch;
            ch = fgetc(file);
        }

        rewind(file);
        fclose(file);
    }
    else { // if file does not exist
        return -1;
    }
    return count;
}

void run_accesses(int base, int bounds, char* filename, int mode, struct access* accesses){
    
    FILE* file = fopen(filename, "r");

    if (file){

        int i = 0;
        char ch = fgetc(file);
        int commented_lines = 0;
        while (ch != EOF){
            
            
            // if line begins with "#", skip to end of line
            if (ch == '#'){ 
                // but keep track of this for an offset
                commented_lines++;
                
                while (ch != '\n') {
                    ch = fgetc(file);
                    if (ch == EOF) {return;}
                }
                ch = fgetc(file);
                continue; 
            }
            
            // On line to be read: write line to string
            int line_size = 30; char* line = malloc(line_size); // Starts at size 20. Will be increased if needed
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
                ch = fgetc(file);

            }
            // Now at end of line
            line[index] = '\0'; // add null terminator

            // tokenize line at spaces and take values into access
            char* op = strtok(line, " ");

            accesses[i].dirty = 0;

            if(strchr(op, 'R')) { accesses[i].o = READ; op = "R";}
            else if(strchr(op, 'W')) { accesses[i].o = WRITE; op = "W";}
            else if(strchr(op, 'X')) { accesses[i].o = EXECUTE; op = "X";}
            else { 
                printf("trace: %s:%d, malformed: op must be R/W, got %s\n", filename, i + commented_lines, op);
                accesses[i].dirty = 1;
                free(line);
                ch = fgetc(file); i++;
                continue;
            }
            
            switch(mode){
                case 1: { // bb

                    char* addr = strtok(NULL, " ");
                    if (!addr) {
                        printf("trace: %s:%d, malformed: expected 'OP ADDR' \n", filename, i + commented_lines);
                        accesses[i].dirty = 1;
                        break;
                    }
                    
                    // check to make sure it is a valid decimal number
                    for (int j = 0; addr[j] != '\0'; j++) {
                        if (!isdigit(addr[j])) {
                            printf("trace: %s:%d, bad address '%s' (not decimal) \n", filename, i + commented_lines, addr);
                            accesses[i].dirty = 1;
                            break;
                        }
                    }
                 

                    accesses[i].address = atoi(addr); 
                    break;
                }
                case 2: { //seg
                    // TODO
                    break;
                }
            }

            if (accesses[i].dirty) {
                free(line);
                ch = fgetc(file); i++;
                continue;
            }

            
            if (accesses[i].address < 0 || accesses[i].address > bounds){
                accesses[i].result = BOUNDS;
            }
            else { accesses[i].result = OK; }
        

            char* result;
            switch (accesses[i].o){
                case 0:
                    op = "R";
                    break;
                case 1:
                    op = "W";
                    break;
            }
            
            switch (accesses[i].result){
                case 0: 
                    result = "ok";
                    break;
                case 1:
                    result = "fault: BOUNDS";
                    break;
            }
            printf("%s %d\t-> PA %d ; %s\n", op, accesses[i].address, accesses[i].address + base, result);
        

            // free line for reuse then move to next line
            free(line);
            ch = fgetc(file); i++;

        }
    }
    return;
    
};

