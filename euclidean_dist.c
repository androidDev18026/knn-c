/* 
 * Reads pairs of (x,y) points from file and calculates the euclidean distance
 * between pairs of point and a boolean adjecency matrix with the points that
 * are close to each other represented with 1, otherwise 0. The similarity of 
 * 2 points is defined by a threshold provided by the user.
 *  
 * Compile : gcc -Wall -std=c99 -g -O3 -lm -o out euclidean_dist.c                 
 * Add < -D _DEBUG > argument for verbose printing of the points,                   
 * the distance and the adjacency matrix.                   
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define DELIM ","

struct Point 
{
  int x;
  int y;
};

// Random initialization of the points - debugging
void init(struct Point* p, unsigned const int size)
{
  assert(size > 0);
	
  for (unsigned int i = 0; i < size; ++i){
    *&(p+i)->x = rand() % 100;
    *&(p+i)->y = rand() % 100;
  }
  puts("Initialized points matrix.");
}

struct Point* read_p_from_file(const char* filename, unsigned const int flen)
{
  FILE* fp = fopen(filename, "r");
	
  char* line;
  size_t len = 0;
  size_t read;
  static unsigned long count = 0;
  
  assert(fp != NULL);
  
  struct Point* points = malloc (flen * sizeof(struct Point));
  if (points == NULL) {
    perror("Allocation failed");
    exit(-1);
  }
 	
  while ((read = getline(&line, &len, fp)) != -1) {
    *&(points+count)->x = atoi(strtok(line, DELIM));
    *&(points+count)->y = atoi(strtok(NULL, DELIM));
    ++count;
   }
	
  free(line);
  fclose(fp);
  
  return points;
}	

float* dist(struct Point p, struct Point* rest, unsigned const int size)
{
  float* euclidean_dist = malloc (size*sizeof(float));
  if (euclidean_dist == NULL) {
    perror("Allocation failed!");
    exit(-1);
  }
	
  for (unsigned int i = 0; i < size; ++i)
  {
    *(euclidean_dist+i) = sqrt(pow(p.x-*&(rest+i)->x,2) + pow(p.y-*&(rest+i)->y,2));
  }
	
  return euclidean_dist;
}

extern inline void show_points(struct Point* points, unsigned const int size)
{
  puts("===Points===");
  for (unsigned int i = 0; i < size; ++i) {
     printf("%-d. (%d,%d)\n", (i+1), *&(points+i)->x, *&(points+i)->y);
  }
}	

extern inline void show_dist_matrix(float** euclidean_dist, unsigned const int size)
{
  puts("============Distance Matrix============");
  for (unsigned int i = 0; i < size; ++i) {
    for (unsigned int j = 0; j < size; ++j) {
      printf("%-7.2f", *(*(euclidean_dist+i) + j));
    }
    puts("");
  } 
}	

static void show_adj_matrix(int** similar, unsigned const int size)
{
  puts("===========Adjacency Matrix============");
  printf("%-3s", "   ");
  for (unsigned int i = 0; i < size; ++i) {
    printf("%-3d", (i+1));
  }
  puts("");
  printf("%-3s", "   ");
  for (unsigned int i = 0; i < size; ++i) {
    printf("---");
  }
  puts("");
  for (unsigned int i = 0; i < size; ++i) {
    printf("%-2d|", (i+1));
    for (unsigned int j = 0; j < size; ++j) {
      printf("%-3d", *(*(similar+i) + j));
    }
    puts("");
  } 
  puts("");
}

unsigned long get_num_points(const char* filename)
{
  FILE* fp = fopen(filename, "r");
  char ch;
  unsigned long nlines = 0;
	
  assert (fp != NULL);
		
  while((ch = fgetc(fp)) != EOF) 
  {
    nlines = ch == '\n' ? nlines + 1 : nlines;
  }
	
  printf("Read %ld pairs of points from file %s.\n", nlines, filename);
	
  fclose(fp);
	
  return nlines;
}

void find_similar(struct Point* points, float** distances, unsigned const int size, 
                    const float threshold, int** similar) 
{
  float* row_dist;
  
  for (unsigned int i = 0; i < size; ++i) {
    row_dist = *(distances + i);
    for (unsigned int j = 0; j < size; ++j) {
       *(*(similar+i)+j) = *(row_dist+j) && *(row_dist+j) < threshold ? 1 : 0;
    }   
  }
}

int main(int argc, const char** argv)
{
	
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <similarity threshold>\n", *argv);
    exit(-1);
  } 
	
  srand(time(NULL));
  
  const char* filename = *(argv+1);
  const float th = atof(*(argv+2)) > 0 ? atof(*(argv+2)) : rand() % 100; 
  unsigned const long SIZE = get_num_points(filename);
  static struct timeval start, end;
  struct Point* points;
  
  points = read_p_from_file(filename, SIZE);
  // init(points, SIZE);
	
  printf("Finding pairs with distance less than %.2f\n", th);
  float** euclidean_dist = malloc(SIZE * sizeof(float*));
  if (euclidean_dist == NULL) {
    perror("Allocation failed!");
    exit(-1);
  }
  
  int** similar = malloc(SIZE*sizeof(int*));
  for (unsigned int i = 0; i < SIZE; ++i) {
    *(similar+i) = malloc(SIZE*sizeof(int));
  }
  if (similar == NULL) {
    perror("Allocation failed");
    exit(-1);
  }
  
  gettimeofday(&start, 0);
  
  for (unsigned int i = 0; i < SIZE; ++i) {
    *(euclidean_dist+i) = dist(*(points+i), points, SIZE);
  }
  puts("Calculated the distance between all points.");
  
  find_similar(points, euclidean_dist, SIZE, th, similar);
  
  gettimeofday(&end, 0);  
   
  #ifdef _DEBUG
  show_points(points, SIZE);
  show_dist_matrix(euclidean_dist, SIZE);
  show_adj_matrix(similar, SIZE);  
  #endif
  
  #ifndef _DEBUG
  float elapsed =  (float) (end.tv_usec - start.tv_usec) / pow(10,6) +
                                    (float) (end.tv_sec - start.tv_sec);
  
  printf("\nTime elapsed(s) : %.3f\n", elapsed);
  puts("==========================");
  #endif
  
  free(points);
  free(similar);
  free(euclidean_dist);
  
  return EXIT_SUCCESS;
}
