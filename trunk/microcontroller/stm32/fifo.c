#include "fifo.h"

void fifo_init(struct fifo *f) {
    f->in = 0;
    f->out = 0;
    f->status = EMPTY;
}

unsigned char fifo_push(struct fifo *f, unsigned char c) {
    if(f->status != FULL) {
        f->buf[f->in] = c;
        f->in = (f->in+1)%FIFO_SIZE;
        if(f->status == EMPTY) f->status = NOT_FULL;
        if(f->in == f->out) f->status = FULL;
        return 0;
    }
    return 1; // error
}

unsigned char fifo_pop(struct fifo *f) {
    unsigned char c;
    if(f->status != EMPTY) {
        c = f->buf[f->out];
        f->out = (f->out+1)%FIFO_SIZE;
        if(f->status == FULL) f->status = NOT_FULL;
        if(f->in == f->out) f->status = EMPTY;
        return c;
    }
    return 1; // error
}

