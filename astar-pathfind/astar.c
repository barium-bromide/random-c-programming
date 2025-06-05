#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "minheap.h"

#define GRID_HEIGHT 5
#define GRID_WIDTH 5
#define GRID_SIZE GRID_HEIGHT * GRID_WIDTH
/**
  * @about manhattan heuristic
  * @return distance to ending position
*/
int32_t heuristic(int32_t pos, int32_t end_pos) {
  int32_t dx = (end_pos % GRID_WIDTH) - (pos % GRID_WIDTH);
  int32_t dy = (end_pos - pos) / GRID_HEIGHT;
  return abs(dx)+ abs(dy);
}

bool contain(min_heap* open, int32_t node) {
  for (int i = 0; i < open->count; i++) {
    if (open->arr[i] == node) {
      return true;
    }
  }
  return false;
}

int32_t* reconstruct_path(int32_t* came_from, int32_t curr) {
  int32_t* path = malloc(GRID_SIZE * sizeof(int32_t));
  path[0] = curr;
  path[GRID_SIZE - 1] = 1;
  while (came_from[curr] != curr) {
    curr = came_from[curr];
    path[path[GRID_SIZE - 1]++] = curr;
  }
  free(came_from);
  return path;
}

void get_neighbour(int32_t* neighbour, int32_t curr_grid_pos, bool* closed, bool* map) {
    if (curr_grid_pos - GRID_WIDTH < 0 || map[curr_grid_pos - GRID_WIDTH] || closed[curr_grid_pos - GRID_WIDTH]) {
      neighbour[0] = -1;
    }
    if (curr_grid_pos % GRID_WIDTH == 0 || map[curr_grid_pos - 1] || closed[curr_grid_pos - 1]) {
      neighbour[1] = -1;
    }
    if ((curr_grid_pos + 1) % GRID_WIDTH == 0 || map[curr_grid_pos + 1] || closed[curr_grid_pos + 1]) {
      neighbour[2] = -1;
    }
    if (curr_grid_pos + GRID_WIDTH > GRID_SIZE - 1 || map[curr_grid_pos + GRID_WIDTH] || closed[curr_grid_pos + GRID_WIDTH]) {
      neighbour[3] = -1;
    }
}

int32_t* astar(int32_t start_pos, int32_t end_pos, bool* map) {
  min_heap* open = create_heap(GRID_SIZE);
  bool* closed = calloc(GRID_SIZE, sizeof(bool));
  assert(closed != NULL);
  int32_t* came_from = malloc(GRID_SIZE * sizeof(int32_t));
  assert(came_from != NULL);
  came_from[start_pos] = start_pos;
  int32_t* g_score = malloc(GRID_SIZE * sizeof(int32_t));
  assert(g_score != NULL);
  for (int32_t i = 0; i < GRID_SIZE; i++) {
    g_score[i] = GRID_SIZE;
  }
  g_score[start_pos] = 0;
  int32_t* f_score = malloc(GRID_SIZE * sizeof(int32_t));
  assert(f_score != NULL);
  for (int32_t i = 0; i < GRID_SIZE; i++) {
    f_score[i] = GRID_SIZE;
  }
  f_score[start_pos] = heuristic(start_pos, end_pos);
  insert(open, start_pos, f_score);

  while (open->count > 0) {
    int32_t curr = find_min(open, f_score);
    closed[curr] = 1;
    if (curr == end_pos) {
      destroy_heap(open);
      free(closed);
      free(g_score);
      free(f_score);
      return reconstruct_path(came_from, curr);
    }
    int32_t neighbour[4] = {curr - GRID_WIDTH, curr - 1, curr + 1, curr + GRID_WIDTH};
    get_neighbour(neighbour, curr, closed, map);
    for (int i = 0; i < 4; i++) {
      if (neighbour[i] == -1) {
	continue;
      }
      //asumming all cost to neighbouring tiles is 1 more
      int32_t new_movement_cost_to_neighbour = g_score[curr] + 1;
      if (new_movement_cost_to_neighbour < g_score[neighbour[i]] || 
      !contain(open, neighbour[i])) {
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
  free(g_score);
  free(f_score);
  return NULL;
}

int32_t main(int32_t arg_c, char** arg_v) {
  bool* map = calloc(GRID_SIZE, sizeof(bool));
  int32_t start = 0;
  int32_t end = GRID_SIZE - 1;
  int32_t* path;
  double endTime;
  double startTime = (float)clock() / CLOCKS_PER_SEC;
  path = astar(start, end, map);
  endTime = (float)clock() / CLOCKS_PER_SEC;
  if (path != NULL) {
    for (int32_t i = 0; i < path[GRID_SIZE - 1]; i++) {
      printf("%d,", path[i]);
    }
    printf("\n");
  } else {
    printf("no");
  };
  printf("%f\n", endTime - startTime);
  return 0;
}
