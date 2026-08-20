#include "task.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void task_create(char *args[], task **head){
    task *novo = (task*)malloc(sizeof(task));
    
    if(novo == NULL){
        printf("memoria falhou");
        return;
    }

    strcpy(novo->nome, args[1]);
    strcpy(novo->programa, args[2]);

    novo->argumentos[0] = '\0';
    for(int i=3;args[i] != NULL;i++){
        strcat(novo->argumentos,args[i]);
        strcat(novo->argumentos," ");
    }

    novo->proximo =NULL;


    if(*head == NULL){
        *head = novo;
    }
    
    else{
        task *atual = *head;

        while(atual->proximo != NULL){
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
}

void imprimir_task(task *head){
    task *atual = head;
    int i = 0;
    while(atual != NULL){
        printf("task[%d]: %s\n", i, atual->nome);
        i++;
        atual = atual->proximo;
    }
}

task *procurar_task(task *head, char nome[]){
    task *atual = head;

    while(atual != NULL && strcmp(atual->nome,nome)!=0 ){
        atual = atual->proximo;
    }

    if(atual ==NULL){
        return NULL;
    }
    return atual;
}