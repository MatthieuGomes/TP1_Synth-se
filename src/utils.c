#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


char* concat_with_necessary_end_null(char* string, ...) {


    va_list args; // list of arguments
    va_start(args, string); // start to read the arguments : args by args readable in the variable called string 
    
    size_t total_length = strlen(string);
    char* current;
    while ((current = va_arg(args, char*)) != NULL) {
        total_length += strlen(current);
    }
    va_end(args); // end of the reading of the arguments

    char* result = (char*)malloc(total_length + 1);

    result[0] = '\0'; 
    strcat(result, string); // start filling the result with the first string

    va_start(args, string); // going again to throught the arguments
    while ((current = va_arg(args, char*)) != NULL) {
        strcat(result, current);
    }
    va_end(args);

    return result;
}