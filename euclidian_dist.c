#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define DELIM ","

struct Point 
{
  int x;
  int y;
};

/*
void init(struct Point* p, const unsigned int size)
{
  assert(size > 0);
	
  for (unsigned int i = 0; i < size; ++i){
    *&(p+i)->x = rand() % 100;
    *&(p+i)->y = rand() % 100;
  }
  puts("Initialized points matrix.");
}
*/

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

float* dist(struct Point p, struct Point* rest, const unsigned int size)
{
  float* euclidian_dist = malloc (size*sizeof(float));
  if (euclidian_dist == NULL) {
    perror("Allocation failed!");
  }
	
  for (unsigned int i = 0; i < size; ++i)
  {
    *(euclidian_dist+i) = sqrt(pow(p.x-*&(rest+i)->x,2) + pow(p.y-*&(rest+i)->y,2));
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


int main(int argc, const char** argv)
{
	
	if (argc != 2) {
		printf("Usage: %s <filename>\n", *argv);
		exit(-1);
	} 
	
	const char* filename = *(argv+1);
	
  unsigned const long SIZE = get_num_lines(filename);
  
  srand(time(NULL));
  
  struct Point* points = read_p_from_file(filename, SIZE);
  
  show_points(points, SIZE);
  
  float** euclidian_dist = malloc(SIZE * sizeof(float*));
  if (euclidian_dist == NULL) {
    perror("Allocation failed!");
  }
  
  puts("============Distance Matrix============");
  for (unsigned int i = 0; i < SIZE; ++i) {
    *(euclidian_dist+i) = dist(*(points+i), points, SIZE);
    for (unsigned int j = 0; j < SIZE; ++j) {
      printf("%-7.2f", *(*(euclidian_dist+i) + j));
    }
    puts("");
  }
  
  free(points);
  free(euclidian_dist);
  
  return EXIT_SUCCESS;
}
