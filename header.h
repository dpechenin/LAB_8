#ifndef LAB8_HEADER_H
#define LAB8_HEADER_H
#include <stdio.h>
#include "flat.h"
#include <string.h>
#include <stdlib.h>
#ifdef __unix__
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <unistd.h>
#else
#include <io.h>
#endif
#define FILE_NAME "database.bin"
#define TEST
#ifdef TEST
void test();
#endif
void add();
flat del(int);
void clearall();
void find();
void edit(int);
void output();
#endif
