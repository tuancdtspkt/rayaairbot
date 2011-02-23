#ifndef FIFO_H
#define FIFO_H

#define FIFO_SIZE 200

#define EMPTY 0
#define NOT_FULL 1
#define FULL 2

struct fifo {
    unsigned char buf[FIFO_SIZE];
    unsigned char in;
    unsigned char out;
    unsigned char status;
};

void fifo_init(struct fifo *f);
unsigned char fifo_push(struct fifo *f, unsigned char c);
unsigned char fifo_pop(struct fifo *f);

#endif

