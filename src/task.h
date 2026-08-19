typedef struct task{
    char nome[50];
    char programa[50];
    char argumentos[100];
    struct task *proximo;
    char input[10000];
    char ouput[10000];
    char append[100000];
}task;

void task_create(char *args[], task **head);

void imprimir_task(task *head);