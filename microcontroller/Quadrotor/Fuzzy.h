#ifndef FUZZY_H
#define FUZZY_H

#include <avr/io.h>

#define NB 0
#define NM 1
#define NS 2
#define ZE 3
#define PS 4
#define PM 5
#define PB 6

struct FUZZY {
  int16_t lastE;
  int16_t e;
  int16_t de;
  uint8_t parabolas[7][2];
  uint8_t reglas_control[7][7];
  int8_t fuzzy_e[7], fuzzy_de[7]; // fuzzyficacion
  int8_t du[4][2]; // reglas que gatillan
  int8_t area[200]; // area resultante
  int16_t r;
};

extern struct FUZZY fuzzy[2];

int Fuzzy (struct FUZZY *f, int e, int de);

#endif
