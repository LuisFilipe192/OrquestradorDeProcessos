#include "job.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/wait.h>
#include <fcntl.h>

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

    int arquivo;

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

        if(jobzao->tarefa->input[0] != '\0'){
            arquivo = open(jobzao->tarefa->input, O_RDONLY);
            
            if(arquivo == -1){
                perror("open");
                exit(1);
            }

            if(dup2(arquivo,0) == -1){
                perror("dup2");
                exit(1);
            }
            close(arquivo);
        }

        if(jobzao->tarefa->output[0] != '\0'){
            arquivo = open(jobzao->tarefa->output, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            
            if(arquivo == -1){
                perror("open");
                exit(1);
            }

            if(dup2(arquivo,1) == -1){
                perror("dup2");
                exit(1);
            }
            close(arquivo);
        }

        if(jobzao->tarefa->append[0] != '\0'){
            arquivo = open(jobzao->tarefa->append, O_WRONLY|O_CREAT|O_APPEND, 0644);
            
            if(arquivo == -1){
                perror("open");
                exit(1);
            }

            if(dup2(arquivo,1) == -1){
                perror("dup2");
                exit(1);
            }
            close(arquivo);
        }

        execv(jobzao->tarefa->programa,argv_exec);

        perror("execv");
        exit(1);


    }
    else{
        jobzao->estado = falhou;
        perror("fork");
    }
}

void job_wait(job *jobzao){
    int status;

    waitpid(jobzao->PID, &status, 0);

        if(WIFEXITED(status)){
            if(WEXITSTATUS(status) == 0){
                jobzao->estado = encerrado;
            }
            else{
                jobzao->estado = falhou;
            }
        }

        if(jobzao->estado == falhou){
            printf("job FALHADO!\n");
        }
        if(jobzao->estado == encerrado){
            printf("job ENCERRADO!\n");
        }
}