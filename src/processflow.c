#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpretador.h"
#include "job.h"

int main(int argc, char *argv[]){

    job *cabeca = NULL;

    task *head = NULL;

    char linha[1000];

    char *args[100];

    int i=0;

    int jobID = 1;

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
            for(i = 0;linha[i] != '\0';i++){
                if(linha[i] == '\n'){
                    linha[i] = '\0';

                    break;
                }
            }
            if(strcmp(linha,"exit") == 0){
                return 0;
            }

            interpretar(linha,args);

            if(strcmp(args[0],"task") == 0){
                if(args[1] != NULL && args[2] != NULL){
                    task_create(args, &head);
                }
            }

            if(strcmp(args[0],"run") == 0){
                if(args[1] != NULL){
                    task *retorno = procurar_task(head,args[1]);

                    if(retorno !=NULL){
                        job *novo = job_create(&cabeca,retorno,jobID);
                        
                        job_execute(novo);

                        jobID++;
                    }
                }

            }
        }
        else{
            return 0;
        }
    }

  
    return 0;
}