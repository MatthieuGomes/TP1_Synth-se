#include <string.h>

ssize_t read_shell(char *input, int max_input_size);
ssize_t print_shell(char *message);
char * concat_with_necessary_end_null(char * string, ...);
int * execute_command(char *command);
char * int_to_str(int number);