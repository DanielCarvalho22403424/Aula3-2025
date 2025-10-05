//
// Created by danie on 01/10/2025.
//


#include "sjf.h"

#include <stdio.h>
#include <stdlib.h>

#include "msg.h"
#include <unistd.h>


void sjf_scheduler(uint32_t current_time_ms, queue_t *rq, pcb_t **cpu_task) {
    /*

    metendo o if cá fora e nao dentro do if(*cpu_task) garantimos que
    a task mais pequena é executada/mandada para o cpu primeiro

    ve se a lista tem mais q um no

    testa se a lista esta ordenada e ordena
    */

    if (rq && rq->head && rq->head->next) {
        int ordenado;
        queue_elem_t *ptr;
        queue_elem_t *last = NULL;

        do {
            //todo
            // nome meio mé pq sai quando já esta ordenado logo ordenado a 0/false ...
            ordenado = 0;
            ptr = rq->head;

            while (ptr->next != last) {
                if (ptr->pcb->time_ms > ptr->next->pcb->time_ms) {

                    /* troca os pcb (apenas os ponteiros), em vez de mexer nos nós
                     * se pcb for null pode quebrar mas acho que nao existe nenhum caso aonde isso seja verdade
                     */

                    pcb_t *tmp = ptr->pcb;
                    ptr->pcb = ptr->next->pcb;
                    ptr->next->pcb = tmp;
                    ordenado = 1;
                }
                ptr = ptr->next;
            }
            last = ptr;
        } while (ordenado);
    }


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
        }
    }
    if (*cpu_task == NULL) {
        // If CPU is idle
        *cpu_task = dequeue_pcb(rq); // Get next task from ready queue (dequeue from head)
    }
}
