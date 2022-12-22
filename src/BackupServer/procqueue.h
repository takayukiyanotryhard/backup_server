#ifndef __PROCQUEUE_H__
#define __PROCQUEUE_H__

#include <stdint.h>

#define LATOR 101

typedef struct str_arg {
    int size;
    unsigned char data[0];
} arg_t;
typedef struct queue {
    int64_t kind;
    int length;
    arg_t * args;
} queue_t;

typedef struct str_iptype {
    int64_t address;
    int type;
}iptype_t;

queue_t* pop();
void push(queue_t*);

#endif 
