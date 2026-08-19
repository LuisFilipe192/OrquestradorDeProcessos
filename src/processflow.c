#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    char linha[1000];

    if(argc == 1){
        printf("modo interativo\n");
    }
    else if(argc == 2){
        printf("modo workflow: %s\n", argv[1]);
    }
    else{
        printf("erro: numero incorreto de argumentos\n");
        return 1;
    }
    
    while(1){
        printf("processflow> ");

        if(fgets(linha,1000,stdin)!=NULL){
            for(int i = 0;linha[i] != '\0';i++){
                if(linha[i] == '\n'){
                    linha[i] = '\0';
                    break;
                }
                if(strcmp(linha,"exit")){
                    return 0;
                }
            }
            
            printf("linha: %s\n", linha);
        }
        else{
            return 0;
        }
    }

    return 0;
}