#ifndef MYLIB_H_
#define MYLIB_H_

#include <stdio.h>
#include <stddef.h>

extern void *emalloc(size_t);
extern void *erealloc(void *, size_t);
extern int getword(char *s, int limit, FILE *stream);

#endif
