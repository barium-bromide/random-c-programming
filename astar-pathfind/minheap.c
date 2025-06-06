#include "minheap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

min_heap *create_heap(int32_t capacity) {
  min_heap *h;
  h = malloc((sizeof *h) + sizeof(int32_t) * capacity);
  h->count = 0;
  h->capacity = capacity;
  return h;
}

void destroy_heap(min_heap *heap) {
  if (heap == NULL) {
    return;
  }
  free(heap);
}

static inline int32_t get_left_child_idx(int32_t parent_idx) {
  return 2 * parent_idx + 1;
}

static inline int32_t get_right_child_idx(int32_t parent_idx) {
  return 2 * parent_idx + 2;
}

static inline int32_t get_parent_idx(int32_t idx) {
  return (int32_t)((idx - 1) / 2);
}

void insert(min_heap *heap, int32_t grid_idx, int32_t *f_score) {
  heap->arr[heap->count] = grid_idx;
  heapify_up(heap, heap->count, f_score);
  heap->count++;
}

static void heapify_up(min_heap *heap, int32_t idx, int32_t *f_score) {
  while (idx != 0 && f_score[heap->arr[get_parent_idx(idx)]] > f_score[heap->arr[idx]]) {
    int32_t temp = heap->arr[idx];
    heap->arr[idx] = heap->arr[get_parent_idx(idx)];
    heap->arr[get_parent_idx(idx)] = temp;
    idx = get_parent_idx(idx);
  }
}

static void heapify_down(min_heap *heap, int32_t idx, int32_t *f_score) {
  int32_t smallest_idx = idx;
  while (true) {
    int32_t left_idx = get_left_child_idx(idx);
    int32_t right_idx = get_right_child_idx(idx);

    if (left_idx < heap->count && f_score[heap->arr[left_idx]] < f_score[heap->arr[smallest_idx]]) {
      smallest_idx = left_idx;
    }
    if (right_idx < heap->count && f_score[heap->arr[right_idx]] < f_score[heap->arr[smallest_idx]]) {
      smallest_idx = right_idx;
    }
    if (smallest_idx != idx) {
      int32_t temp = heap->arr[idx];
      heap->arr[idx] = heap->arr[smallest_idx];
      heap->arr[smallest_idx] = temp;
      idx = smallest_idx;
    } else {
      break;
    }
  }
}

int32_t find_min(min_heap *heap, int32_t *f_score) {
  if (heap->count == 0) {
    return -1;
  }
  int32_t max = heap->arr[0];
  heap->count--;
  heap->arr[0] = heap->arr[heap->count];
  heapify_down(heap, 0, f_score);
  return max;
}
