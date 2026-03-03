// Elijah Greig
// 3128908
// Submitted on: Feb 15 2025
// File: vmsim.c | Source file for vmsim application

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void usage(){
    printf("Usage: ./vmsim \n --mode=bb  --base=N --limit=N --trace=FILE \n --mode=seg --config=FILE --trace=FILE \n");
}

int main(){
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
    }
    else if (strcmp(argv[1], "--mode=seg") == 0){
        mode = 2;
    }
    else if (!mode){
        printf("Mode provided was not valid.\n")
        usage();
        return 1;
    }

    switch (mode){

        // bb
        case 1:
            int base;
            int limit;
            char* filename;

            // ensure next argument is base
            if (strncmp(argv[2], "--base=", 7) == 0){ // comparing first 7 characters of the arg string for correct usage    
                if (!isdigit(argv[2]+7)) {
                    printf("Base provided was not a valid number.\n")
                    usage(); return 1;
                }
                base = atoi(argv[2]+7);
            }
            

            // ensure next argument is limit
            if (strncmp(argv[3], "--limit=", 8) == 0){ // comparing first 7 characters of the arg string for correct usage    
                if (!isdigit(argv[3]+8)) {
                    printf("Limit provided was not a valid number.\n")
                    usage(); return 1;
                }
                limit = atoi(argv[3]+8);
            }
            else { usage(); return 1; }


            // ensure next argument is trace
            if (strncmp(argv[4], "--trace=", 8) == 0){ // comparing first 5 characters of the arg string for correct usage
                filename = malloc(strlen(argv[4] + 8) + 1); // moves pointer to count length of string past equals sign
                if (filename == NULL) {
                        printf("Memory allocation failed");
                        return 1;
                    } 
                strcpy(filename, argv[4] + 8);
                // TODO: check file exists
            }
            else { usage(); return 1; }

        break;

        // seg
        case 2:
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
                // TODO: check file exists
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
                // TODO: check file exists
            }
            else { usage(); return 1; }

            // Echo file
            char line[100]; // To be dynamically allocated
            FILE* trace = fopen(filename_trace, "r");
            FILE* config = fopen(filename_config, "r");

            fgets(line, sizeof(line), trace);
            while (line){
                if (line[0] != '#') {printf("%s", line)} 
                fgets(line, sizeof(line), trace);
            }

            fgets(line, sizeof(line), config);
            while (line){
                if (line[0] != '#') {printf("%s", line)} 
                fgets(line, sizeof(line), config);
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