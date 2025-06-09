#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#define NDEBUG
#include <assert.h>

typedef struct {
  int32_t count;
  int32_t capacity;
  int32_t arr[];
} Min_heap;

void insert(Min_heap *heap, int32_t item, int32_t *f_score);
int32_t find_min(Min_heap *heap, int32_t *f_score);
Min_heap *create_heap(size_t capacity);
void destroy_heap(Min_heap *heap);

#endif
