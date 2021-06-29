/* Reads pairs of (x,y) points from file and calculates the euclidian distance between the points 
 *
 * Compile : gcc -Wall -std=c99 -g -O3 -lm -o out main.c                 
 * Add < -D _DEBUG > for verbose printing of the points,                   
 * the distance and the adjacency matrix.                   
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
void init(struct Point* p, const unsigned int size)
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
  float* euclidian_dist = malloc (size*sizeof(float));
  if (euclidian_dist == NULL) {
    perror("Allocation failed!");
    exit(-1);
  }
	
  for (unsigned int i = 0; i < size; ++i)
  {
    *(euclidian_dist+i) = sqrt(pow(p.x-*&(rest+i)->x,2) + pow(p.y-*&(rest+i)->y,2));
  }
	
  return euclidian_dist;
}

void show_points(struct Point* points, unsigned const int size)
{
  puts("===Points===");
  for (unsigned int i = 0; i < size; ++i) {
     printf("%d. (%d,%d)\n", (i+1), *&(points+i)->x, *&(points+i)->y);
  }
}

unsigned long get_num_lines(const char* filename)
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
	 
  return nlines;
}

void find_similar(struct Point* points, float** distances, unsigned const int size, 
                    const float threashold, int** similar) 
{
  float* row_dist;
  
  for (unsigned int i = 0; i < size; ++i) {
    row_dist = *(distances + i);
    for (unsigned int j = 0; j < size; ++j) {
       *(*(similar+i)+j) = *(row_dist+j) && *(row_dist+j) < threashold ? 1 : 0;
    }   
  }
  
}

int main(int argc, const char** argv)
{
	
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <similarity threashold>\n", *argv);
    exit(-1);
  } 
	
  srand(time(NULL));
  
  const char* filename = *(argv+1);
  const float th = atof(*(argv+2)) > 0 ? atof(*(argv+2)) : rand() % 100; 
  unsigned const long SIZE = get_num_lines(filename);
  static struct timeval start, end;
  
  struct Point* points = read_p_from_file(filename, SIZE);
  // init(points, SIZE);
	
  printf("Finding pairs with distance less than %.2f\n", th);
  float** euclidian_dist = malloc(SIZE * sizeof(float*));
  if (euclidian_dist == NULL) {
    perror("Allocation failed!");
    exit(-1);
  }
  
  int** similar = malloc(SIZE * sizeof(int*));
  for (unsigned int i = 0; i < SIZE; ++i) {
    *(similar+i) = malloc(SIZE*sizeof(int));
  }
  if (similar == NULL) {
    perror("Allocation failed");
    exit(-1);
  }
  
  gettimeofday(&start, 0);
  
  for (unsigned int i = 0; i < SIZE; ++i) {
    *(euclidian_dist+i) = dist(*(points+i), points, SIZE);
  }
  
  find_similar(points, euclidian_dist, SIZE, th, similar);
  
  gettimeofday(&end, 0);  
   
  #ifdef _DEBUG
  show_points(points, SIZE);
  
  puts("============Distance Matrix============");
  for (unsigned int i = 0; i < SIZE; ++i) {
    for (unsigned int j = 0; j < SIZE; ++j) {
      printf("%-7.2f", *(*(euclidian_dist+i) + j));
    }
    puts("");
  } 
  puts("===========Adjacency Matrix============");
  printf("%-3s", "   ");
  for (unsigned int i = 0; i < SIZE; ++i) {
    printf("%-3d", (i+1));
  }
  puts("");
  printf("%-3s", "   ");
  for (unsigned int i = 0; i < SIZE; ++i) {
    printf("---");
  }
  puts("");
  for (unsigned int i = 0; i < SIZE; ++i) {
    printf("%-2d|", (i+1));
    for (unsigned int j = 0; j < SIZE; ++j) {
      printf("%-3d", *(*(similar+i) + j));
    }
    puts("");
  } 
  
  #endif
  
  #ifndef _DEBUG
  float elapsed =  (float) (end.tv_usec - start.tv_usec) / pow(10,6) +
                                    (float) (end.tv_sec - start.tv_sec);
  
  puts("=====================");
  printf("Time elapsed %.3fs\n", elapsed);
  puts("=====================");
  #endif
  
  free(points);
  free(similar);
  free(euclidian_dist);
  
  return EXIT_SUCCESS;
}

