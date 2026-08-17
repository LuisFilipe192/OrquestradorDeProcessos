#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if(argc == 1){
        printf("modo interativo\n");
        return 0;
    }
    else if(argc == 2){
        printf("modo workflow: %s\n", argv[1]);
        return 0;
    }
    else{
        printf("erro: numero incorreto de argumentos\n");
        return 1;
    }

    return 0;
}