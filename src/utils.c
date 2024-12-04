#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#define concat(...) concat_with_necessary_end_null(__VA_ARGS__, NULL) // define the macro concat to call the function concat_with_necessary_end_null with the NULL argument at the end
#define KILL_SIGNAL 9


ssize_t print_shell(char *message){
    return write(STDOUT_FILENO, message, strlen(message));
}

ssize_t read_shell(char *input, int max_input_size){
    ssize_t input_size = read(STDIN_FILENO,input,max_input_size);
    // ends correctly the string with a null character to avoid any buffer overflow
    input[input_size-1] = '\0';
    return input_size;
}
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

int * execute_command(char *command){
    pid_t pid = fork();
    // checks if the fork was successful OR if the process is the child
    if(pid <= 0){
        // checks if the fork was successful
        if(pid < 0){
            perror("Fork Error");
        }
        // checks if the successfully forked process has error during execution
        else
        {
            pid_t current_pid = getpid();
            if(strcmp(command,"signal9")==0){
                kill(current_pid,KILL_SIGNAL);
            }
            else{
                execlp(command,command,NULL);
                perror("Command Error");
            }
            
        }
        exit(EXIT_FAILURE);
    }
    else{
        int status;
        waitpid(pid,&status,0);
        int code;
        int is_signaled = WIFSIGNALED(status);
        if(is_signaled){
            code = WTERMSIG(status);
        }
        else{
            code = WEXITSTATUS(status);
        }
        int * response = malloc(sizeof(code)+sizeof(is_signaled));
        response[0] = is_signaled; 
        response[1] = code;
        return response;
    }
}

char * int_to_str(int number){
    char code_as_string_ptr[] = "\0";
    sprintf(code_as_string_ptr,"%d",number);
    char * code_as_string= malloc(strlen(code_as_string_ptr));
    strcpy(code_as_string,code_as_string_ptr);
    return code_as_string;
}
