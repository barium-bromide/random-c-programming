#ifndef MINHEAP_H
#define MINHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct {
  int32_t count;
  int32_t capacity;
  int32_t arr[];
} min_heap;

void insert(min_heap *heap, int32_t item, int32_t *f_score);
int32_t find_min(min_heap *heap, int32_t *f_score);
min_heap *create_heap(int32_t capacity);
void destroy_heap(min_heap *heap);

#ifdef __cplusplus
}
#endif

#endif
