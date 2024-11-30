#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "enseash.h"

#include "shell_utils.h"
#include "utils.h"

#define concat(...) concat_with_necessary_end_null(__VA_ARGS__, NULL) // define the macro concat to call the function concat_with_necessary_end_null with the NULL argument at the end

#define MAX_INPUT_SIZE 1024 // Choose a maximum input size pretty large to avoid problems with large inputs

char exit_key_name[] = "CTRL+D";
char exit_key_char[] = {61,-62,-96,1}; // (CTRL+D) Kamoulox : there is no way not to use magic numbers here ¯\_(ツ)_/¯
// A cleaner way to do this, would be to have a map of the key names and their ascii values... but that's way to overkill.
char exit_command[] = "exit";
char prompt_title[] = "enseash";
char prompt_suffix[] = "%";
char exit_message[] = "Bye bye...\n";


int main(){
    print_shell(concat("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'", exit_command,"\' ou pressez ",exit_key_name," \n"));       
    return REPL();
}

int REPL(){
    char *exec_infos=NULL;
    
    while(1){
        char input[MAX_INPUT_SIZE];
        char *prompt_base=NULL;
        char *prompt_message=NULL;
        if(exec_infos != NULL){
            prompt_base=concat(prompt_title," ",exec_infos);
        }
        else{
            prompt_base=prompt_title;
        }
        prompt_message = concat(prompt_base, " ",prompt_suffix," ");
        print_shell(prompt_message);
        read_shell(input,MAX_INPUT_SIZE);
        if(strncmp(input,exit_command,strlen(input)) == 0 || strncmp(input,exit_key_char,strlen(input))==0){
            if (strncmp(input,exit_key_char,strlen(input))==0){
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
            char code_as_string[]="\0";
            sprintf(code_as_string,"%d",exit_code_cmd);
            exec_infos = concat("[exit:",code_as_string,"]");
        }
        free(prompt_base);
        free(prompt_message);
    }
    free(exec_infos);
    

    return EXIT_SUCCESS;
}