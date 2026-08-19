#include "interpretador.h"
#include <string.h>

void interpretar(char linha[], char *args[]){
    char *token;
    int j=0;
    

    token = strtok(linha," ");
    while(token!=NULL){
        args[j]=token;
        j++;
        token = strtok(NULL," ");
    }
    args[j]=NULL;
            
}