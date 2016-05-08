/*
 * Lecture d'un image, seuillage, puis écriture.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void process_image();

int main(int argc, char **argv)
{
    char *inName, *outName;
    int seuil;

    if(argc > 2)
    {
        unsigned char *buf_in, *buf_out;
        int dimx, dimy; /* nombre de colonnes, de lignes de l'image */
        int x, y;

        /* parametres de la commande */
        inName=argv[1];
        outName=argv[2];
        seuil=atoi(argv[3]);
        
        /* lecture image */
        buf_in=read_grayscale(inName, &dimx, &dimy);

        /* traitement sur l'image */
        buf_out=(unsigned char *)malloc(dimx*dimy);
        process_image(buf_out, buf_in, dimx, dimy, seuil);

        /* ecriture image */
        write_grayscale(outName, dimx, dimy, buf_out);
        
        free(buf_in);
        free(buf_out);
    }
    else
        printf("Usage: %s image-in image-out seuil\n", *argv);
    return 0;
}

/* Exemple d'un traitement très simple: seuillage d'une image 
 * Illustre l'accès à un pixel donné dans le tampon 
 */

void process_image (unsigned char *buf_out, unsigned char *buf_in, int dimx, int dimy, int seuil)
{
    int i, j;
    for(j=0;j<dimy;j++)
        for(i=0;i<dimx;i++)
            buf_out[i+dimx*j]=(buf_in[i+dimx*j] > seuil)*255;
  
}

