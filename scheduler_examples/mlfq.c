//
// Created by danie on 05/10/2025.
//
#include "mlfq.h"

#include <stdio.h>
#include <stdlib.h>

#include "msg.h"
#include <unistd.h>



// organiza a fila pela prioridade
// e ve se a @task oque está agr no cpu nao é a que tem  prioridade mais baixa
int enqueue_pcb_sorted(queue_t* q, pcb_t* task) {
    if (!q || !task){
      return 0;
    }
    queue_elem_t* elem = malloc(sizeof(queue_elem_t));

    if (!elem){
      return 0;
    }

    elem->pcb = task;
    elem->next = NULL;

    // listaa vazia
    if (!q->head) {
        //para tail parar de ser null
        q->head = q->tail = elem;
        return 1;
    }

    // mete o com menor prioridade no inicioo
        if (task->priority < q->head->pcb->priority) {
        elem->next = q->head;
        q->head = elem;
        return 1;
    }

    // mantem a ordem de fifo e chama a funcao caso tenham a mms prioridade "dequeue_pcb"
    queue_elem_t* prev = q->head;
    queue_elem_t* current = q->head->next;
    while (current) {
        if (task->priority < current->pcb->priority) {
            prev->next = elem;
            elem->next = current;
            return 1;
        }
        prev = current;
        current = current->next;
    }

    // mete o resto no tail
    q->tail->next = elem;
    q->tail = elem;
    return 1;
}




// time-slice = 500 milisegundos

void mlfq_scheduler(uint32_t current_time_ms, queue_t *rq, pcb_t **cpu_task) {
    if (*cpu_task) {
        (*cpu_task)->ellapsed_time_ms += TICKS_MS; // Add to the running time of the application/task

        if ((*cpu_task)->ellapsed_time_ms >= (*cpu_task)->time_ms) {
            // Task finished
            // Send msg to application
            msg_t msg = {
                .pid = (*cpu_task)->pid,
                .request = PROCESS_REQUEST_DONE,
                .time_ms = current_time_ms
            };
            if (write((*cpu_task)->sockfd, &msg, sizeof(msg_t)) != sizeof(msg_t)) {
                perror("write");
            }
            // Application finished and can be removed (this is FIFO after all)
            free((*cpu_task));
            (*cpu_task) = NULL;
        } else {
            // ve se já foram passados os 500ms
            if ((((*cpu_task)->ellapsed_time_ms % (TIME_SLICE)) == 0) && (*cpu_task)->time_ms != 0) {
              (*cpu_task)->priority += 1;

                //ve se a lista nao esta vazia
                if (!queuIsEmpty(rq)) {
                    // tenta meter no fim
                    if (enqueue_pcb_sorted(rq, *cpu_task) == 0) {
                        perror("enqueue_pcb_sorted ERROR");
                    } else {
                        // mete o prox. a ocupar o cpu basicamente o (*cpu_task == NULL)
                        *cpu_task = dequeue_pcb(rq);
                    }
                }
            }
        }
    }

    if (*cpu_task == NULL) {
        // If CPU is idle
        *cpu_task = dequeue_pcb(rq); // Get next task from ready queue (dequeue from head)
    }
}
