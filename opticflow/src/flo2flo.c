/*
 * Lecture d'un fichier de vitesse, calcul de statistique, écriture du fichier
 */
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void print_stats( float *, int, int);

int main( int argc, char **argv) {
  if( argc > 2) {
    float *buf;
    int dimx, dimy;

    buf = read_velocities( argv[1], &dimx, &dimy);

    print_stats( buf, dimx, dimy);
    
    write_velocities( argv[2], dimx, dimy, buf);
    free( buf);
    return 0;
  } else
    return 1;
}

void print_stats( float *buf, int dimx, int dimy) {
  int x, y;
  float mu, mv;

  mu = mv = 0;
  for( y=0; y<dimy; y++)
    for( x=0; x<dimx; x++) {
      mu += buf[0]; mv += buf[1];
      buf += 2;
    }
  fprintf( stderr, "%f %f\n", mu/(dimx*dimy), mv/(dimx*dimy));
}
