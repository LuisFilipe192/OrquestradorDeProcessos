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
}

void pipe_executar(task *head,char **args,job **cabeca, int *jobID){

    int i;
    int quantidade = 0;
    int quantidade_pipes;
    int resultado;

    for(i=2;args[i] !=NULL;i++){
        quantidade++;
    }

    quantidade_pipes = quantidade -1;

    int (*pipes)[2];
    pipes = malloc(quantidade_pipes * sizeof(*pipes)); 

    if(pipes == NULL){
        printf("erro: memoria falhou\n");
        return;
    }
    
    for(int i=0;i< quantidade_pipes;i++){
        if(pipe(pipes[i]) == -1){
            printf("pipe");
            return;
        }
    }

    job *primeiro_pipe = NULL;

    for(i=2;args[i] !=NULL;i++ ){

        task *retorno = procurar_task(head,args[i]);

        if(retorno == NULL){
            printf("erro: tarefa nao encontrada\n");
            return;
        }
    }

    for(i = 2; args[i] != NULL; i++){

        task *retorno = procurar_task(head,args[i]);

        if(retorno == NULL){
            printf("erro: tarefa nao encontrada\n");
            free(pipes);
            return;
        }

        job *novo = job_create(cabeca,head,*jobID);

        if(novo == NULL){
            free(pipes);
            return;
        }

        if(primeiro_pipe == NULL){
            primeiro_pipe = novo;
        }

        (*jobID)++;


        resultado = fork();

        if(resultado == -1){
            perror("fork");
            free(pipes);
            return;
        }

        else if(resultado > 0){
            novo->PID = resultado;
            novo->estado = executando;
        }

        else if(resultado == 0){

            if(i == 2){
                if(dup2(pipes[0][1], 1) == -1){
                    perror("dup2");
                    exit(1);
                }
            }

            else if(i < quantidade+1){
                if(dup2(pipes[i-3][0],0) == -1){
                    perror("dup2");
                    exit(1);
                }

                if(dup2(pipes[i-2][1],1) == -1){
                    perror("dup2");
                    exit(1);
                }
            }

            else if(i == quantidade + 1){
                if(dup2(pipes[quantidade_pipes-1][0],0) == -1){
                    perror("dup2");
                    exit(1);
                }
            }

            for(int j=0; j < quantidade_pipes;j++){
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            char *argv_exec[100];
            char *token;
            int k = 1;

            argv_exec[0] = retorno->programa;

            token = strtok(retorno->argumentos," ");

            while(token != NULL){
                argv_exec[k] = token;
                k++;

                token = strtok(NULL, " ");
            }

            argv_exec[k] = NULL;

            execv(retorno->programa, argv_exec);

            perror("execv");
            exit(1);

            }
        }

        for(i = 0; i < quantidade_pipes; i++){
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        free(pipes);

        job *atual = primeiro_pipe;

        while(atual != NULL){
            job_wait(atual);
            atual = atual->proximo;
        }
    
}
