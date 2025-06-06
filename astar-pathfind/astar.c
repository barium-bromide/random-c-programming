#include "minheap.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_HEIGHT 10000
#define GRID_WIDTH 10000
#define GRID_SIZE GRID_HEIGHT * GRID_WIDTH
#define MAX_PATH_SIZE ((GRID_HEIGHT / 2) * (GRID_WIDTH + 1) + (GRID_HEIGHT & 1 ? GRID_WIDTH: GRID_WIDTH - (GRID_WIDTH / 2) - 1))
/**
 * @about manhattan heuristic
 * @return distance to ending position
 */
static int32_t heuristic(int32_t pos, int32_t end_pos) {
  int32_t dx = (end_pos % GRID_WIDTH) - (pos % GRID_WIDTH);
  int32_t dy = (end_pos - pos) / GRID_HEIGHT;
  return abs(dx) + abs(dy);
}

static bool contain(min_heap *open, int32_t node) {
  for (int i = 0; i < open->count; i++) {
    if (open->arr[i] == node) {
      return true;
    }
  }
  return false;
}

static int32_t *reconstruct_path(int32_t *came_from, int32_t curr) {
  size_t num_of_elem = MAX_PATH_SIZE + 1;
  int32_t *path = malloc(num_of_elem * sizeof(int32_t));
  path[0] = 2;
  path[1] = curr;
  while (came_from[curr] != curr) {
    curr = came_from[curr];
    path[path[0]++] = curr;
  }
  free(came_from);
  return path;
}

static void get_neighbour(int32_t *neighbour, int32_t curr_grid_pos, bool *closed, bool *map) {
  int32_t up = curr_grid_pos - GRID_WIDTH;
  int32_t down = curr_grid_pos + GRID_WIDTH;
  int32_t left = curr_grid_pos - 1;
  int32_t right = curr_grid_pos + 1;
  neighbour[0] = (up < 0 || map[up] || closed[up]) ? -1 : up;
  neighbour[1] = (curr_grid_pos % GRID_WIDTH == 0 || map[left] || closed[left]) ? -1 : left;
  neighbour[2] = ((curr_grid_pos + 1) % GRID_WIDTH == 0 || map[right] || closed[right]) ? -1 : right;
  neighbour[3] = (down >= GRID_SIZE || map[down] || closed[down]) ? -1 : down;
}

int32_t *astar(int32_t start_pos, int32_t end_pos, bool *map) {
  min_heap *open = create_heap(GRID_SIZE);
  bool *closed = calloc(GRID_SIZE, sizeof(bool));
  int32_t *came_from = malloc(3 * GRID_SIZE * sizeof(int32_t));
  int32_t *g_score = came_from + GRID_SIZE;
  int32_t *f_score = g_score + GRID_SIZE;
  if (open == NULL || closed == NULL || came_from == NULL) {
    return NULL;
  }
  came_from[start_pos] = start_pos;
  for (int32_t i = 0; i < GRID_SIZE; i++) {
    f_score[i] = GRID_SIZE;
    g_score[i] = GRID_SIZE;
  }
  g_score[start_pos] = 0;
  f_score[start_pos] = heuristic(start_pos, end_pos);
  insert(open, start_pos, f_score);

  while (open->count > 0) {
    int32_t curr = find_min(open, f_score);
    closed[curr] = 1;
    if (curr == end_pos) {
      destroy_heap(open);
      free(closed);
      return reconstruct_path(came_from, curr);
    }
    int32_t neighbour[4];
    get_neighbour(neighbour, curr, closed, map);
    for (int i = 0; i < 4; i++) {
      if (neighbour[i] == -1) {
        continue;
      }
      // asumming all cost to neighbouring tiles is 1 more
      int32_t new_movement_cost_to_neighbour = g_score[curr] + 1;
      if (new_movement_cost_to_neighbour < g_score[neighbour[i]] || !contain(open, neighbour[i])) {
        came_from[neighbour[i]] = curr;
        g_score[neighbour[i]] = new_movement_cost_to_neighbour;
        f_score[neighbour[i]] = g_score[neighbour[i]] + heuristic(neighbour[i], end_pos);

        if (!contain(open, neighbour[i])) {
          insert(open, neighbour[i], f_score);
        }
      }
    }
  }
  destroy_heap(open);
  free(closed);
  free(came_from);
  return NULL;
}

int32_t main(int32_t arg_c, char **arg_v) {
  bool *map = calloc(GRID_SIZE, sizeof(bool));
  int32_t start = 0;
  int32_t end = GRID_SIZE - 1;
  int32_t *path;
  double endTime;
  double startTime = (float)clock() / CLOCKS_PER_SEC;
  path = astar(start, end, map);
  endTime = (float)clock() / CLOCKS_PER_SEC;
  printf("%f\n", endTime - startTime);
  return 0;
}
