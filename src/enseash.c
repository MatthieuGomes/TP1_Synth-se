#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "enseash.h"

#include "shell_utils.h"
#include "utils.h"

#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoid problems with large inputs

char prompt_title[] = "enseash";
char prompt_suffix[] = "%";

char exit_key_name[] = "CTRL+D";
char exit_key_char[] = {61,-62,-96,1}; // (CTRL+D) Kamoulox : there is no way not to use magic numbers here ¯\_(ツ)_/¯
// A cleaner way to do this, would be to have a map of the key names and their ascii values... but that's way to overkill.
char exit_command[] = "exit";
char exit_message[] = "Bye bye...\n";


int main(){
    print_shell(generate_welcome_message(exit_command,exit_key_name));       
    return REPL();
}

int REPL(){
    char *prompt_infos=NULL;
    
    while(1){
        char input[MAX_INPUT_SIZE];
        char * prompt_message=generate_prompt_message(prompt_title,prompt_suffix,prompt_infos);
        print_shell(prompt_message);
        ssize_t input_size=read_shell(input,MAX_INPUT_SIZE);
        if(strcmp(input,"\0")==0&&input_size!=0){
            continue;
        }
        else if(strncmp(input,exit_command,strlen(input)) == 0 || input_size==0){
            if (input_size==0){
                print_shell("\n"); // When using CTRL+D, the shell does not print a new line, so we do it manually
            }
            print_shell(exit_message);
            return EXIT_SUCCESS;
        }
        else{ // this else is not necessary but it is here to make the code more readable
            int exit_code_cmd = execute_command(input);
            if(exit_code_cmd == EXIT_FAILURE){
                print_shell("La commande a échoué, réessayez\n");
            }
            prompt_infos = generate_prompt_infos(exit_code_cmd);
        }
    }
    free(prompt_infos);
    return EXIT_SUCCESS;
}