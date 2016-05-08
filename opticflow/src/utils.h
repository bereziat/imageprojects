/**
 * Utilitaires divers entrée/sortie
 */

#include <Imlib2.h>

unsigned char * read_grayscale  (char *fname, int *dimx, int *dimy);

void            write_grayscale (char *fname, int dimx, int dimy, unsigned char *buf);
void            write_color     (char *fname, int dimx, int dimy, unsigned char *buf);

float *         read_velocities ( char *fname, int *dimx, int *dimy);
void            write_velocities( char *fname, int dimx, int dimy, float *buf);

