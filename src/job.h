#include <sys/types.h>
#include "task.h"

typedef enum {
    executando,
    encerrado,
    falhou,
    standby
} estadojob;

typedef struct job{
    int jobID;
    pid_t PID;
    estadojob estado;
    task *tarefa;
    struct job *proximo;
}

job *job_create(job **cabeca,task *tarefa, int jobID;);

