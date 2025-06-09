#include "minheap.h"

Min_heap *create_heap(size_t capacity) {
  Min_heap *h = malloc((sizeof *h) + sizeof(int32_t) * capacity);
  h->count = 0;
  h->capacity = capacity;
  return h;
}

void destroy_heap(Min_heap *heap) {
  assert(heap);

  free(heap);
}

static inline int32_t left_child_idx(int32_t parent_idx) {
  return 2 * parent_idx + 1;
}

static inline int32_t right_child_idx(int32_t parent_idx) {
  return 2 * parent_idx + 2;
}

static inline int32_t parent_idx(int32_t idx) {
  return (int32_t)((idx - 1) / 2);
}

static void heapify_up(Min_heap *heap, int32_t idx, int32_t *f_score) {
  while (idx != 0 && f_score[heap->arr[parent_idx(idx)]] > f_score[heap->arr[idx]]) {
    int32_t temp = heap->arr[idx];
    heap->arr[idx] = heap->arr[parent_idx(idx)];
    heap->arr[parent_idx(idx)] = temp;
    idx = parent_idx(idx);
  }
}

static void heapify_down(Min_heap *heap, int32_t idx, int32_t *f_score) {
  int32_t smallest_idx = idx;
  while (true) {
    int32_t left_idx = left_child_idx(idx);
    int32_t right_idx = right_child_idx(idx);

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

void insert(Min_heap *heap, int32_t grid_idx, int32_t *f_score) {
  heap->arr[heap->count] = grid_idx;
  heapify_up(heap, heap->count, f_score);
  heap->count++;
}

int32_t find_min(Min_heap *heap, int32_t *f_score) {
  if (heap->count == 0) {
    return -1;
  }
  int32_t max = heap->arr[0];
  heap->count--;
  heap->arr[0] = heap->arr[heap->count];
  heapify_down(heap, 0, f_score);
  return max;
}
