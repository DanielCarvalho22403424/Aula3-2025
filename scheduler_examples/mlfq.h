//
// Created by danie on 05/10/2025.
//

#ifndef MLFQ_H
#define MLFQ_H

#include "queue.h"
#include <stdint.h>

void mlfq_scheduler(uint32_t current_time_ms, queue_t *rq, pcb_t **cpu_task);


#endif //MLFQ_H
