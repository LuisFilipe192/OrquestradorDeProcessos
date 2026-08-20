#include "job.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

job *job_create(job **cabeca,task *tarefa,int jobID){
    job *novo = (job*)malloc(sizeof(job));

    if(novo == NULL){
        printf("memoria falhou");
        return NULL;
    }

    novo->tarefa = tarefa;
    novo->estado = standby;
    novo->jobID = jobID;
    novo->proximo = NULL;

    if(*cabeca == NULL){
        *cabeca = novo;
    }
    else{
        job *atual = *cabeca;
        while(atual->proximo != NULL){
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }

    return novo;
}

void job_execute(job *jobzao){
    int resultado = fork();
    int i;

    if(resultado>0){
        jobzao->PID = resultado;
        jobzao->estado = executando;
    }
    else if(resultado == 0){
        char *argv_exec[100];
        char *token;
        argv_exec[0] = jobzao->tarefa->programa;
        i = 1;
        token = strtok(jobzao->tarefa->argumentos," ");

        while(token!=NULL){
            argv_exec[i]=token;
            i++;
            token = strtok(NULL," ");
        }
        argv_exec[i] = NULL;

        execv(jobzao->tarefa->programa,argv_exec);

        perror("execv");
        exit(1);


    }
    
}