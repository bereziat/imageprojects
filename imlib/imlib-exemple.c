/*
 * D.Béréziat (c) 2011 pour UPMC.
 *
 * Routine de chargement/traitement/sauvegarde d'image simplifiée
 * avec imlib2: un grand nombre de formats sont gérés.
 * Installation sur Ubuntu/Debian: copiez-coller la ligne suivante dans le terminal:
 *  sudo apt-get install libimlib2-dev
 * Installation pour Mac: copiez-coller les deux lignes suivantes dans le terminal:
 *  curl /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
 *  brew install imlib2
 * Compilation:
 *  gcc imlib-exemple.c -o imlib-exemple `imlib2-config --cflags --libs`
 * ou encore :
 *  make
 */

#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * Lit une image (type quelconque) et la convertie en niveaux de gris 
 */
unsigned char * read_grayscale(char *fname, int *dimx, int *dimy)
{
    Imlib_Image *image;

    image = (Imlib_Image *)imlib_load_image(fname);
    if(image)
    {
        unsigned char *buf, *data;
        int l,c;

        imlib_context_set_image( image);
        *dimx = imlib_image_get_width();
        *dimy = imlib_image_get_height();
        data  = (unsigned char *)imlib_image_get_data();
        buf = (unsigned char*) malloc(sizeof(char)**dimx**dimy);
        for(l=0;l<*dimy;l++)
	  for(c=0;c<*dimx;c++)
            {
                *buf++ = (*data + *(data+1) + *(data+2))/3;
                data += 4;
            }
        imlib_free_image();
        return buf-*dimx**dimy;
    }
    else
    {
        fprintf( stderr, "Erreur: image %s n'a pu etre ouverte.\n", fname);
        exit(33);
    }
}

/*
 * Ecrit une image sur le disque. L'extension du nom de fichier
 * donne le format: Par exemple: toto.jpg -> l'image sera écrite
 * au format Jpeg.
 */

void write_grayscale(char *fname, int dimx, int dimy, unsigned char *buf)
{
    Imlib_Image *image;
    unsigned char *data;
    int l, c;
    char *ext;

    if( !(ext = strchr( fname, '.')))
    {
        fprintf( stderr, "Erreur: format image non reconnu\n");
        exit(34);
    }

    data = (unsigned char *)malloc(sizeof(char)*dimx*dimy*4);
    for( l=0; l<dimy; l++)
      for( c=0; c<dimx; c++)
        {
            *data = *buf;
            *(data+1)= *buf;
            *(data+2) = *buf;
            *(data+3) = 0;
            buf ++; data += 4;
        }
    data -= dimx*dimy*4;
    image = (Imlib_Image *)imlib_create_image_using_data( dimx, dimy, (DATA32*)data);
    imlib_context_set_image( image);
    imlib_image_set_format(ext+1);
    imlib_save_image( fname);
    imlib_free_image();
    free( data);
}

/*
 * Un exemple d'utilisation des deux fonctions précédentes.
 */

void process_image();


int main(int argc, char **argv)
{
    char *inName, *outName;
    double seuil;
    if(argc > 3)
    {
        unsigned char *buf_in, *buf_out;
        int dimx, dimy; /* nombre de colonnes, de lignes de l'image */
        int x, y;

        /* parametres de la commande */
        inName=argv[1];
        outName=argv[2];
        seuil = atof(argv[3]);
        
        /* lecture image */
        buf_in = read_grayscale(inName, &dimx, &dimy);

        /* traitement sur l'image */
        buf_out = (unsigned char *)malloc(dimx*dimy);
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

void process_image (unsigned char *buf_out, unsigned char *buf_in, int dimx, int dimy, double seuil)
{
    int i, j;
    for(j=0;j<dimy;j++)
        for(i=0;i<dimx;i++)
            buf_out[i+dimx*j]=(buf_in[i+dimx*j] > seuil)*255;
  
}

/**
 * Ecrit une image couleur (rgb) sur le disque. L'extension du nom de fichier
 * donne le format: Par exemple: toto.jpg -> l'image sera écrite
 * au format Jpeg.
 */

void write_color(char *fname, int dimx, int dimy, unsigned char *R, unsigned char *G, unsigned char *B) {
  Imlib_Image *image;
  unsigned char *data;
  char *ext;
  int l;
  
  if( !(ext = strchr( fname, '.'))) {
    fprintf( stderr, "Erreur: format image non reconnu\n");
    exit(34);
  }
  
  data = (unsigned char *)malloc(sizeof(char)*dimx*dimy*4);
  if( !buf) {
    fprintf( stderr, "Erreur: plus de memoire libre (allocation de %ld octets).\n",
	     dimx*dimy*sizeof(float));
    exit(36);
  }
  for( l=0; l<dimy; l++) {
    int c;
    for( c=0; c<dimx; c++) {
      *data = *R++;
      *(data+1)= *G++;
      *(data+2) = *B++;
      *(data+3) = 0;
      data += 4;
    }
  }
  data -= dimx*dimy*4;
  image = imlib_create_image_using_data( dimx, dimy, (DATA32*)data);
  imlib_context_set_image( image);
  imlib_image_set_format( ext+1);
  imlib_save_image( fname);
  imlib_free_image();
  free( data);  
}
