// Elijah Greig
// 3128908
// Submitted on: 
// File: parse.c | Source file for vmsim application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vmsim.h"


int count_accesses(char* filename){
    FILE* file = fopen(filename, "r");
    
    if (file){
        int count = 0;
        char buffer[100]; // does not need to be dynamically allocated, will hold the whole line (or a portion of it) with fgets()

        while(fgets(buffer, 100, file)){ // runs until EOF

            if (strchr(buffer, '\n') == NULL && !feof(file)) { // checks if there is no newline in the buffer and that we are not at the end of the file
                // means only a fragment of the whole line was taken
                continue; // line was longer than buffer.
            }
            if(buffer[0] != '#') {
                count++; // add to access count
            }

            

        }
        return count;

    }
    return -1; // failed to access file
}

void get_accesses(char* filename, int mode, struct access* accesses){
    
    FILE* file = fopen(filename, "r");

    if (file){

        int i = 0;
        char ch = fgetc(file);
        while (ch != EOF){
            
            
            // if line begins with "#", skip to end of line
            if (ch == '#'){ 
                
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

            if(strchr(op, 'R')) { accesses[i].o = READ;}
            else if(strchr(op, 'W')) { accesses[i].o = WRITE;}
            else if(strchr(op, 'X')) { accesses[i].o = EXECUTE;}

            switch(mode){
                case 1: { // bb
                    accesses[i].address = atoi(strtok(NULL, " ")); 
                    break;
                }
                case 2: { //seg
                    // TODO
                    break;
                }
            }

            // free line for reuse then move to next line
            free(line);
            ch = fgetc(file); i++;

        }
    }
    return;
    
};

