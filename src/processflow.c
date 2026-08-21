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

    int erro_sequential = 0;
    int erro_parallel = 0;

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

            if(args[0] == NULL){
                continue;
            }

            if(strcmp(args[0],"task") == 0){
                task *retorno = procurar_task(head, args[1]);
                if(retorno!=NULL){
                    printf("erro: tarefa ja cadastrada\n");
                    
                }
                
                else{
                    if(args[1] != NULL && args[2] != NULL){
                        task_create(args, &head);
                    }
                }
            }

            if(strcmp(args[0],"run") == 0){

                if(args[1] == NULL){
                    printf("erro: tarefa não informada\n");
                    continue;
                }

                if(strcmp(args[1],"sequential")== 0){
                    erro_sequential=0;
                    if(args[2] == NULL){
                        printf("erro: tarefa não informada\n");
                        continue;
                    }
                    else{
                        for(i=2;args[i] !=NULL;i++){
                            task *retorno = procurar_task(head,args[i]);
                            if(retorno==NULL){
                                printf("erro: tarefa não existe\n");
                                erro_sequential = 1;
                            }
                            if(erro_sequential==1){
                                continue;
                            }

                            else{
                                job *novo = job_create(&cabeca,retorno,jobID);

                                job_execute(novo);
                                job_wait(novo);

                                jobID++;
                            }
                        }
                    }
                }

                else if(strcmp(args[1],"parallel")== 0){
                    erro_parallel = 0;

                    if(args[2] == NULL){
                        printf("erro: tarefa nao informada\n");
                        continue;
                    }

                    else{
                        for(i=2;args[i] != NULL;i++){
                            task *retorno = procurar_task(head, args[i]);

                            if(retorno ==NULL){
                                printf("erro: tarefa não existe\n");
                                erro_parallel = 1;
                            }
                        }

                        if(erro_parallel==1){
                            continue;
                        }

                        job *ultimo = cabeca;
                        job *atual;

                        while(cabeca != NULL && ultimo->proximo!=NULL){
                            ultimo = ultimo->proximo;
                        }

                        for(i=2;args[i] != NULL;i++){
                            task *retorno =procurar_task(head,args[i]);
                            if(retorno==NULL){
                                printf("erro: tarefa não existe\n");
                                continue;
                            }

                            else{
                                job *novo = job_create(&cabeca,retorno,jobID);

                                job_execute(novo);

                                jobID++;
                            }
                        }

                        if(ultimo == NULL){
                            atual = cabeca;
                        }

                        else{
                            atual = ultimo->proximo;
                        }

                        while(atual !=NULL){
                            job_wait(atual);
                            atual = atual->proximo;
                        }
                    }
                }

                    else if(strcmp(args[1],"pipe") == 0){    
                        if(args[2] == NULL && args[3] == NULL){
                            printf("erro: tarefas não informadas\n");
                            continue;
                        }
                        pipe_executar(head,args,&cabeca,&jobID);

                    }

                    else if(args[1] != NULL){
                    task *retorno = procurar_task(head,args[1]);

                    if(retorno !=NULL){
                        job *novo = job_create(&cabeca,retorno,jobID);
                        
                        job_execute(novo);
                        job_wait(novo);

                        jobID++;
                    }
                
                }   
            }

            else if(strcmp(args[0], "start") == 0){
                if(args[1] == NULL){
                    printf("erro: tarefa nao informada\n");
                    continue;
                }

                task *retorno =procurar_task(head,args[1]);

                if(retorno !=NULL){
                    job* novo = job_create(&cabeca,retorno,jobID);

                    job_execute(novo);
                    jobID++;
                    
                    printf("[%d] %d\n",novo->jobID,novo->PID);
                }
            }
            else if(strcmp(args[0], "jobs") == 0){
                job *atual = cabeca;

                while(atual != NULL){
                    if(atual->estado == executando){
                        printf("[%d] %d -> executando\n",atual->jobID,atual->PID);
                    }
                    else if(atual->estado == encerrado){
                        printf("[%d] %d -> encerrado\n",atual->jobID,atual->PID);
                    }
                    else if(atual->estado == falhou){
                        printf("[%d] %d -> falhoun",atual->jobID,atual->PID);
                    }
                    atual = atual->proximo;
                }
            }
            else if(strcmp(args[0], "wait") == 0){
                job *atual = cabeca;
                while(atual != NULL){
                    if(atual->jobID==atoi(args[1])){
                        job_wait(atual);
                        break;
                    }
                    else{
                        atual=atual->proximo;
                    }
                }
                if(atual == NULL){
                    printf("erro: job nao encontrado\n");
                }
            }

            else if(strcmp(args[0],"input") == 0){
                if(args[1] != NULL && args[2] != NULL ){
                    task *retorno = procurar_task(head, args[1]);

                    if(retorno!=NULL){
                    strcpy(retorno->input,args[2]);
                    }
                }
            }
            else if(strcmp(args[0],"output") == 0){
                if(args[1] != NULL && args[2] != NULL ){
                    task *retorno = procurar_task(head, args[1]);

                    if(retorno!=NULL){
                    strcpy(retorno->output,args[2]);
                    }
                }
            }

            else if(strcmp(args[0],"append") == 0){
                if(args[1] != NULL && args[2] != NULL ){
                    task *retorno = procurar_task(head, args[1]);

                    if(retorno!=NULL){
                    strcpy(retorno->append,args[2]);
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
