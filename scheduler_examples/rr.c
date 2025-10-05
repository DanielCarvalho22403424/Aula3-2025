//
// Created by danie on 01/10/2025.
//


#include "rr.h"

#include <stdio.h>
#include <stdlib.h>

#include "msg.h"
#include <unistd.h>



// time-slice = 500 milisegundos

void rr_scheduler(uint32_t current_time_ms, queue_t *rq, pcb_t **cpu_task) {
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
                //ve se a lista nao esta vazia
                if (!queuIsEmpty(rq)) {
                    // tenta meter no fim
                    if (enqueue_pcb(rq, *cpu_task) == 0) {
                        perror("enqueue_pcb ERROR");
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
