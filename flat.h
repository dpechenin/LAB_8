#ifndef LAB8_FLAT_H
#define LAB8_FLAT_H
#include "fio.h"
typedef struct {
    unsigned int flatNum;
    char type[100];
    fio fio;
    unsigned int debtsUtilityBills;
    unsigned int amountResidents;
} flat;
#endif
