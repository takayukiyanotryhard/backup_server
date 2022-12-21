#ifndef __PROCQUEUE_H__
#define __PROCQUEUE_H__

#include <stdint.h>

#define LATOR 101

typedef struct queue {
    int64_t kind;
} queue_t;

queue_t* pop();
void push(queue_t*);

#endif 
