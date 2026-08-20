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

                if(strcmp(args[1],"sequential")== 0){
                    for(i=2;args[i] !=NULL;i++){
                        task *retorno = procurar_task(head,args[i]);

                        if(retorno != NULL){
                            job *novo = job_create(&cabeca,retorno,jobID);

                            job_execute(novo);
                            job_wait(novo);

                            jobID++;
                        }
                    }
                }

                else if(strcmp(args[1],"parallel")== 0){

                    job *ultimo = cabeca;
                    job *atual;

                    while(cabeca != NULL && ultimo->proximo!=NULL){
                        ultimo = ultimo->proximo;
                    }
                    for(i=2;args[i] != NULL;i++){
                        task *retorno =procurar_task(head,args[i]);

                        if(retorno != NULL){
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
                        printf("[%d] %d -> falhou\\\\\\\n",atual->jobID,atual->PID);
                    }
                    
                    atual = atual->proximo;
                }

            }
            else if(strcmp(args[0], "wait") == 0){
        }
        else{
            return 0;
        }
    }
        return 0;
    }
