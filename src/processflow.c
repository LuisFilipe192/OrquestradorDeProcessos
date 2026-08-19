#include <stdio.h>
#include <stdlib.h>

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
            printf("leitura completa\n");
        }
        else{
            return 0;
        }

    }

    return 0;
}