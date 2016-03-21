#include <pthread.h>

typedef struct {
    int threadId;
    uint8_t *pixels;
    light_node lights;
    rectangular_node rectangulars;
    sphere_node spheres;
    color background;
    const viewpoint *view;
    int rowStart;    
    int colStart;
    int rowEnd;
    int colEnd;
} Thread_Arg;
