
#ifndef __PRECALC_H
#define __PRECALC_H
#if defined(__cplusplus)
extern "C" {
#endif
#include "engine2048.h"

void precalc_init();

extern int precalc_directions[SIZE][4];
extern int precalc_directions_last[SIZE][4];
extern int precalc_directions_first[SIZE][4];

#if defined(__cplusplus)
}
#endif

#endif
