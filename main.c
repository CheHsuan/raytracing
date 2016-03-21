#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "primitives.h"
#include "raytracing.h"
#include "thread_d.h"

#define OUT_FILENAME "out.ppm"

#define ROWS 512
#define COLS 512

static void write_to_ppm(FILE *outfile, uint8_t *pixels,
                         int width, int height)
{
    fprintf(outfile, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(pixels, 1, height * width * 3, outfile);
}

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main()
{
    uint8_t *pixels;
    light_node lights = NULL;
    rectangular_node rectangulars = NULL;
    sphere_node spheres = NULL;
    color background = { 0.0, 0.1, 0.1 };
    struct timespec start, end;

    pthread_t threadId[4];
    Thread_Arg th_args[4];
    int thread_num;
    do{
  	printf("Enter thread num (must be the factor of 512) : ");
    	scanf("%d",&thread_num);
	getchar();
    }while(512%thread_num != 0);

#include "use-models.h"
    int p_start = 0;
    int num = (ROWS/1) / thread_num;
    pixels = malloc(sizeof(unsigned char) * ROWS * COLS * 3);
    /* allocate by the given resolution */
    if (!pixels) exit(-1);

    for(int i = 0; i < thread_num; i++){
	th_args[i].threadId = i;
    	th_args[i].pixels = pixels;
        th_args[i].lights = lights;
	th_args[i].rectangulars = rectangulars;
	th_args[i].spheres = spheres;
	th_args[i].background[0] = background[0];
	th_args[i].background[1] = background[1];
	th_args[i].background[2] = background[2];
	th_args[i].view = &view;
	th_args[i].rowStart = p_start;
        th_args[i].colStart = 0;
        th_args[i].rowEnd = p_start + num;
        th_args[i].colEnd = COLS;
        p_start += num;
    }

    printf("# Rendering scene\n");
    /* do the ray tracing with the given geometry */
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i = 0; i < thread_num; i++)
        pthread_create(&(threadId[i]), NULL, (void *)raytracing, (void *)&th_args[i]);
    for(int i = 0; i < thread_num; i++)
	pthread_join(threadId[i], NULL);
    clock_gettime(CLOCK_REALTIME, &end);
    {
        FILE *outfile = fopen(OUT_FILENAME, "wb");
        write_to_ppm(outfile, pixels, ROWS, COLS);
        fclose(outfile);
    }
    delete_rectangular_list(&rectangulars);
    delete_sphere_list(&spheres);
    delete_light_list(&lights);
    free(pixels);
    printf("Done!\n");
    printf("Execution time of raytracing() : %lf sec\n", diff_in_second(start, end));
    exit(0);
}
