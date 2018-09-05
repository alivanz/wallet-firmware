#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

typedef void (*func_call)();

typedef struct{
  const char *name;
  func_call fx;
  const char *desc;
} func_t;

#define concat(...) __VA_ARGS__

#define XX(tag,x,...) printf(tag " " x "\n", ##__VA_ARGS__)
#define LOG(x, ...)   XX("LOG", x, ## __VA_ARGS__)
#define ERROR(x, ...) XX("ERROR", x, __VA_ARGS__)
#define DATA(x, ...)  XX("DATA", x, __VA_ARGS__)
#define OK(x, ...)    XX("OK", x, __VA_ARGS__)

void command();

#endif
