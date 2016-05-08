/**
 * colorcode.cpp
 *
 * Color encoding of flow vectors
 * adapted from the color circle idea described at
 *  http://members.shaw.ca/quadibloc/other/colint.htm
 *
 * Daniel Scharstein, 4/2007
 * added tick marks and out-of-range coding 6/05/07
 */

#include <stdlib.h>
#include <math.h>

#include "colorcode.h"

int ncols = 0;
#define MAXCOLS 60
int colorwheel[MAXCOLS][3];

#ifndef M_PI
# define M_PI		3.14159265358979323846
#endif

void setcols(int r, int g, int b, int k)
{
    colorwheel[k][0] = r;
    colorwheel[k][1] = g;
    colorwheel[k][2] = b;
}

void makecolorwheel()
{
    // relative lengths of color transitions:
    // these are chosen based on perceptual similarity
    // (e.g. one can distinguish more shades between red and yellow 
    //  than between yellow and green)
    int RY = 15;
    int YG = 6;
    int GC = 4;
    int CB = 11;
    int BM = 13;
    int MR = 6;
    ncols = RY + YG + GC + CB + BM + MR;
    //printf("ncols = %d\n", ncols);
    if (ncols > MAXCOLS)
	exit(1);
    int i;
    int k = 0;
    for (i = 0; i < RY; i++) setcols(255,	   255*i/RY,	 0,	       k++);
    for (i = 0; i < YG; i++) setcols(255-255*i/YG, 255,		 0,	       k++);
    for (i = 0; i < GC; i++) setcols(0,		   255,		 255*i/GC,     k++);
    for (i = 0; i < CB; i++) setcols(0,		   255-255*i/CB, 255,	       k++);
    for (i = 0; i < BM; i++) setcols(255*i/BM,	   0,		 255,	       k++);
    for (i = 0; i < MR; i++) setcols(255,	   0,		 255-255*i/MR, k++);
}

void computeColor(float fx, float fy, unsigned char *pix)
{
    if (ncols == 0)
	makecolorwheel();

    float rad = sqrt(fx * fx + fy * fy);
    float a = atan2(-fy, -fx) / M_PI;
    float fk = (a + 1.0) / 2.0 * (ncols-1);
    int k0 = (int)fk;
    int k1 = (k0 + 1) % ncols;
    float f = fk - k0;
    //f = 0; // uncomment to see original color wheel
    for (int b = 0; b < 3; b++) {
	float col0 = colorwheel[k0][b] / 255.0;
	float col1 = colorwheel[k1][b] / 255.0;
	float col = (1 - f) * col0 + f * col1;
	if (rad <= 1)
	    col = 1 - rad * (1 - col); // increase saturation with radius
	else
	    col *= .75; // out of range
	pix[2 - b] = (int)(255.0 * col);
    }
}

#define UNKNOWN_FLOW_THRESH 1e9

int unknown_flow(float u, float v) {
  return (fabs(u) >  UNKNOWN_FLOW_THRESH) 
    || (fabs(v) >  UNKNOWN_FLOW_THRESH)
    || isnan(u) || isnan(v);
}

/**
 * Converti un tampon de vitesse (couple de valeurs par pixel) en
 * une image RGB (triplet de valeurs par pixel) en utilisant la fonction
 * computeColor() défini dans le module colorcode.c.
 */

void velocities2color( unsigned char *color, float *vel, int dimx, int dimy) {
    int x, y;

    // determine motion range:
    float maxx = -999, maxy = -999;
    float minx =  999, miny =  999;
    float maxrad = -1;
    for (y = 0; y < dimy; y++) {
	for (x = 0; x < dimx; x++) {
	  float fx = *vel++;
	  float fy = *vel++;
	  
	  if (unknown_flow(fx, fy))
	    continue;
	  maxx = fmax(maxx, fx);
	  maxy = fmax(maxy, fy);
	  minx = fmin(minx, fx);
	  miny = fmin(miny, fy);
	  float rad = sqrt(fx * fx + fy * fy);
	  maxrad = fmax(maxrad, rad);
	}
    }

    /*
      fprintf( stderr, "max motion: %.4f  motion range: u = %.3f .. %.3f;  v = %.3f .. %.3f\n",
	       maxrad, minx, maxx, miny, maxy);

        
    if (maxmotion > 0) // i.e., specified on commandline
      maxrad = maxmotion;
    */
    
    if (maxrad == 0) // if flow == 0 everywhere
      maxrad = 1;
    
    vel -= dimx*dimy*2;

    for (y = 0; y < dimy; y++) {
      for (x = 0; x < dimx; x++) {
	float fx = *vel ++;
	float fy = *vel ++;
	unsigned char *pix = color ;
	if (unknown_flow(fx, fy)) {
	  pix[0] = pix[1] = pix[2] = 0;
	} else {
	  computeColor(fx/maxrad, fy/maxrad, pix);
	}
	color += 3;
      }
    }
}

