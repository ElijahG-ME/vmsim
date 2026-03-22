// Elijah Greig
// 3128908
// Submitted on: Mar 22 2026
// File: parse.c | Source file for vmsim application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vmsim.h"
#include "parse_bb.h"


int count_accesses(char* filename){ // Count lines in file
    
    FILE* file;
    int count = 0;
    int last_char = '\n';
    if (file = fopen(filename, "r")){

        char ch = fgetc(file); // evaluates file character by character
        while (ch != EOF){ 
            if (last_char == '\n') {
                if (ch != '#') {count += 1;} // if line beginning with '#' is met, entire line is skipped
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

void run_accesses_bb(int base, int bounds, char* filename, struct access* accesses){
    
    FILE* file = fopen(filename, "r");

    if (file){

        int i = 0;
        char ch = fgetc(file);
        int commented_lines = 0;
        while (ch != EOF){
            
            
            // if line begins with "#", skip to end of line
            if (ch == '#'){ 
                // but keep track of this for an offset when printing
                commented_lines++;
                
                while (ch != '\n') {
                    ch = fgetc(file);
                    if (ch == EOF) {return;}
                }
                ch = fgetc(file);
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
                ch = fgetc(file);

            }
            // Now at end of line
            line[index] = '\0'; // add null terminator

            // tokenize line at spaces and take values into access
            char* op = strtok(line, " ");

            accesses[i].dirty = 0;

            // Declare specified operation of access
            if(strchr(op, 'R')) { accesses[i].o = READ; op = "R";}
            else if(strchr(op, 'W')) { accesses[i].o = WRITE; op = "W";}
            else if(strchr(op, 'X')) { accesses[i].o = EXECUTE; op = "X";}

            else { // if invalid, print issue and mark as dirty
                printf("trace: %s:%d, malformed: op must be R/W, got %s\n", filename, i + commented_lines, op);
                accesses[i].dirty = 1;
                free(line);
                ch = fgetc(file); i++;
                continue;
            }
            
            

            char* addr = strtok(NULL, " "); // record address
            if (!addr) { // if address is missing, print issue & mark as dirty
                printf("trace: %s:%d, malformed: expected 'OP ADDR' \n", filename, i + commented_lines);
                accesses[i].dirty = 1;
                break;
            }
            
            // check to make sure it is a valid decimal number
            
            if (!is_number(addr)) { // if invalid, print issue & mark as dirty
                printf("trace: %s:%d, bad address '%s' (not decimal) \n", filename, i + commented_lines, addr);
                accesses[i].dirty = 1;
            }
            
            if (!accesses[i].dirty) { accesses[i].address = atoi(addr); } // record address if not dirty
                    

            // Begin printing output

            if (accesses[i].dirty) { // skip access entirely if marked dirty
                free(line);
                ch = fgetc(file); i++;
                continue;
            }

            
            if (accesses[i].address < 0 || accesses[i].address > bounds){ // evaluate for bounds fault
                accesses[i].result = BOUNDS;
            }
            else { accesses[i].result = OK; }
        

            char* result;
            switch (accesses[i].o){ // mark operation into char for printing
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
                    printf("%s %d\t-> PA %d ; %s\n", op, accesses[i].address, accesses[i].address + base, result);
                    break;
                case 1:
                    result = "fault: BOUNDS";
                    printf("%s %d\t-> %s\n", op, accesses[i].address, result);
                    break;
            }
        

            // free line for reuse then move to next line
            free(line);
            ch = fgetc(file); i++;

        }
    }
    fclose(file);
    return;
    
};

