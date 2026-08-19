#include "job.h"

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