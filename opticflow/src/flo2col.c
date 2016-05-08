#include <stdlib.h>
#include "colorcode.h"
#include "utils.h"

#include <inrimage/image.h>

int main( int argc, char **argv) {
  if( argc > 2) {
    float *vel;
    unsigned char *col;
    int dimx, dimy;

    vel = read_velocities( argv[1], &dimx, &dimy);
    col = (unsigned char *)malloc(3*dimx*dimy);
    velocities2color( col, vel, dimx, dimy);
    write_color( argv[2], dimx, dimy, col);
    free( col);
    free( vel);
    return 0;
  }
  return 1;
}
