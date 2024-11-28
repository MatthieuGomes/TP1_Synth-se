# TP1_Synth-se
TP1 de synthèse par Matthieu GOMES et Oumaima EL BOUROUMI

## Question 1

Nous avons séparé le code en 2 partie pour commencer : 
- La première partie est situé dans le fichier qui donne son nom au programme (enseash) et contient la fonction main. Cette dernière appelle la fonction `print_welcome_message()` définie dans le second fichier.
```c title="enseash.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "enseash.h"

int main(){
    print_welcome_message();        
}

```
*enseash.c*
```c title="enseash.h"
void print_welcome_message();
```
*enseash.h*
- Le second fichier (functions) contient pour le moment uniquement la fonction `print_welcome_message()` qui affiche un message de bienvenue.


On definie une taille max d'input pour la fonction `read` très grande pour ne pas avoir de problème de taille. Le reste se contente de suivre les consignes et d'utiliser les fonctions `write` et `read` pour afficher les messages et lire l'input de l'utilisateur.
```c title="functions.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "functions.h"
#define MAX_INPUT_SIZE 1024
char *welcome_message = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez \'exit\'\n";
char *prompt_message = "enseash % ";


void print_welcome_message(){
    char input[MAX_INPUT_SIZE];
    write(STDOUT_FILENO, welcome_message, strlen(welcome_message));
    write(STDOUT_FILENO, prompt_message, strlen(prompt_message));
    read(STDIN_FILENO,input,strlen(input));
}
```
*functions.c*
```c title="functions.h"
void print_welcome_message();
```
*functions.h*