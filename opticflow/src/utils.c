/**
 * Utilitaires divers entrée/sortie
 */

#include <Imlib2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Lit une image (type quelconque) et la convertie en niveaux de gris 
 */
unsigned char * read_grayscale(char *fname, int *dimx, int *dimy)
{
  Imlib_Image *image;

  image = imlib_load_image(fname);
  if(image) {
    unsigned char *buf, *data;
    int l,c;
    
    imlib_context_set_image( image);
    *dimx = imlib_image_get_width();
    *dimy = imlib_image_get_height();
    data = (unsigned char *)imlib_image_get_data();
    buf = (unsigned char*) malloc(sizeof(char)**dimx**dimy);
    if( !buf) {
      fprintf( stderr, "Erreur: plus de memoire libre (allocation de %ld octets).\n",
	       *dimx**dimy*sizeof(float));
      exit(36);
    }
    
    for(l=0;l<*dimy;l++)
      for(c=0;c<*dimx;c++) {
	*buf++ = (*data + *(data+1) + *(data+2))/3;
	data += 4;
      }
    imlib_free_image();
    return buf-*dimx**dimy;
  } else {
    fprintf( stderr, "Erreur: image %s n'a pu etre ouverte.\n", fname);
    exit(33);
  }
}

/**
 * Ecrit une image (niveau de gris )sur le disque. L'extension du nom de fichier
 * donne le format: Par exemple: toto.jpg -> l'image sera écrite
 * au format Jpeg.
 */
void write_grayscale(char *fname, int dimx, int dimy, unsigned char *buf) {
  Imlib_Image *image;
  unsigned char *data;
  int l, c;
  char *ext;
  
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
  image = imlib_create_image_using_data( dimx, dimy, (DATA32*)data);
  imlib_context_set_image( image);
  imlib_image_set_format(ext+1);
  imlib_save_image( fname);
  imlib_free_image();
  free( data);
}

/**
 * Ecrit une image (rgb) sur le disque. L'extension du nom de fichier
 * donne le format: Par exemple: toto.jpg -> l'image sera écrite
 * au format Jpeg.
 * Format rgb: chaque pixel a un triplet de valeurs (rouge, vert, bleu) codées
 * sur 1 octet.
 */

void write_color(char *fname, int dimx, int dimy, unsigned char *buf) {
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
      *data = *buf;
      *(data+1)= *(buf+1);
      *(data+2) = *(buf+2);
      *(data+3) = 0;
      buf +=3; data += 4;
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

/**
 * Lit un fichier de vitesse (Format Middlebury).
 * Format: chaque pixel a un couple de valeurs (composante horizontale,
 * composante verticale) codées sur un flottant.
 * Si fname == '<', on lit dans l'entrée standard
 */ 

float * read_velocities ( char *fname, int *dimx, int *dimy) {
#define TAG_FLOAT 202021.25
  float tag, *buf;  
  FILE *fp = *fname == '<' ? stdin : fopen( fname, "rb");
  long l;
  
  if ((int)fread(&tag,  sizeof(float), 1, fp) != 1 ||
      (int)fread(dimx, sizeof(int),   1, fp) != 1 ||
      (int)fread(dimy, sizeof(int),   1, fp) != 1 ||
      tag != TAG_FLOAT
      ) {
    fprintf( stderr, "Erreur: format (%s) velocites non reconnu.\n", fname);
    exit(35);
  }

  buf = (float *)malloc(2*sizeof(float)**dimx**dimy);
  if( !buf) {
    fprintf( stderr, "Erreur: plus de memoire libre (allocation de %ld octets).\n",
	     2**dimx**dimy*sizeof(float));
    exit(36);
  }
  
  if( (l=fread( buf, 2*sizeof(float), *dimx**dimy, fp)) != *dimx**dimy) {
    if( fp != stdin) fclose( fp);
    fprintf( stderr, "Erreur: fichier %s tronqué: il manque %ld octets\n",
	     fname, *dimx**dimy*sizeof(float)-l);
    exit(37);
  }
  if( fp != stdin) fclose( fp);
  return buf;
}

/**
 * Ecrit un fichier de vitesse (format Middlebury)
 * Si fname == '>', on écrit dans la sortie standard.
 */

void write_velocities( char *fname, int dimx, int dimy, float *buf) {
  FILE *fp = *fname == '>' ? stdout : fopen( fname, "wb");
  float tag = TAG_FLOAT;

  fwrite( &tag, sizeof(float), 1, fp);
  fwrite( &dimx, sizeof(int), 1, fp);
  fwrite( &dimy, sizeof(int), 1, fp);
  fwrite( buf, 2*sizeof(float), dimx*dimy, fp);
  if( fp != stdout) fclose(fp);  
}

