#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>

struct Point 
{
  int x;
  int y;
};

void init(struct Point* p, const unsigned int size)
{
  assert(size > 0);
	
  for (unsigned int i = 0; i < size; ++i){
    *&(p+i)->x = rand() % 100;
    *&(p+i)->y = rand() % 100;
  }
  puts("Initialized points matrix.");
}

float* dist(struct Point p, struct Point* rest, const unsigned int size)
{
  float* euclidian_dist = malloc(size*sizeof(float));
  if (euclidian_dist == NULL) {
    perror("Allocation failed!");
  }
	
  for (unsigned int i = 0; i < size; ++i)
  {
    euclidian_dist[i] = sqrt(pow(p.x-*&(rest+i)->x,2) + pow(p.y-*&(rest+i)->y,2));
  }

  return euclidian_dist;
}

void show_points(struct Point* points, const unsigned int size)
{
  puts("===Points===");
  for (unsigned int i = 0; i < size; ++i) {
     printf("%d. (%d,%d)\n", (i+1), *&(points+i)->x, *&(points+i)->y);
  }
}

/*
int* add(struct Point* p, const unsigned int size)
{
  int* concat = malloc(size*sizeof(int));
  if (concat == NULL) {
    perror("Allocation failed!");
  }
  
  for (unsigned int i = 0; i < size; ++i) {
    concat[i] = (p[i].x+p[i].y);
  }
  
  return concat;
}
*/

int main(int argc, const char** argv)
{
  static const unsigned int SIZE = 10;
  
  srand(time(NULL));
  
  struct Point* points = (struct Point*) malloc (SIZE * sizeof(struct Point));
  if (points == NULL) {
		perror("Allocation failed!");
	}
	
  init(points, SIZE);
  show_points(points, SIZE);
  
  float** euclidian_dist = malloc(SIZE * sizeof(float*));
  if (euclidian_dist == NULL) {
		perror("Allocation failed!");
  }
  
  puts("============Distance============");
  for (unsigned int i = 0; i < SIZE; ++i) {
    *(euclidian_dist+i) = dist(*(points+i), points, SIZE);
    for (unsigned int j = 0; j < SIZE; ++j) {
      printf("%-7.2f", *(*(euclidian_dist+i) + j));
    }
    puts("");
  }
  
  free(points);
  free(euclidian_dist);
  
  return 0;
}
