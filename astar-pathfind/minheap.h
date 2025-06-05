#ifndef minheap_h
#define minheap_h

#include <stdint.h>
typedef struct {
  int32_t* arr;
  int32_t count;
  int32_t capacity;
} min_heap;

static inline int32_t get_left_child_idx(int32_t parent_idx);
static inline int32_t get_right_child_idx(int32_t parent_idx);
static inline int32_t get_parent_idx(int32_t idx);
void insert(min_heap* heap, int32_t item, int32_t* f_score);
static void heapify_up(min_heap* heap, int32_t idx, int32_t* f_score);
static void heapify_down(min_heap* heap, int32_t idx, int32_t* f_score);
int32_t find_min(min_heap* heap, int32_t* f_score);
min_heap* create_heap(int32_t capacity);
void destroy_heap(min_heap* heap);

#endif
