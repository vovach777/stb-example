#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


uint8_t * rgb = NULL;
size_t    rgb_size;
int       width;
int       stride;
int       height;

static inline int clip_c( int c )
{
   if (c < 0)
      return 0;
   if (c > 255)
      return 255;
   return c;
}
static inline int make_rgb( int r, int g, int b) {

   return clip_c(r) | clip_c(g) << 8 | clip_c(b) << 16;

}



static void load_rgb(char * img)
{
   int  num_channels;

  unsigned char* bitmap = stbi_load(img, &width, &height, &num_channels, 0);
  if (bitmap == NULL || width * height == 0) {
      printf("load image error!\n");
      exit(1);
  }
  rgb_size = width*height*3;
  stride = width*3;
  rgb = realloc(rgb,rgb_size);

  switch (num_channels)  {
   case 1: {
      uint8_t *rgb_p = rgb;
      uint8_t *gray8_p = bitmap;
      size_t n=width*height;
      while (n--) {
         const uint8_t g8 = *gray8_p++;
         *rgb_p++ = g8;
         *rgb_p++ = g8;
         *rgb_p++ = g8;
      }
      break;
      }
   case 3:
      memcpy(rgb,bitmap,rgb_size);
      break;
   case 4: {
      uint8_t *rgb_p = rgb;
      uint8_t *bitmap_p = bitmap;
      size_t n=width*height;
      while (n--) {
         *rgb_p++ = bitmap_p[0];
         *rgb_p++ = bitmap_p[1];
         *rgb_p++ = bitmap_p[2];
         bitmap_p += 4;
      }
      break;
   }
   default:
         printf("number of channels == %d - is not support!!!\n", num_channels);
         exit(1);
  }
  free(bitmap);
}

static void save_rgb(char * img)
{
   size_t i = strlen(img)-1;
   if (i > 3) {
      if (
         tolower(img[i-2]) == 'p' &&
         tolower(img[i-1]) == 'n' &&
         tolower(img[i])   == 'g') {
         stbi_write_png(img, width,height, 3, rgb, stride);
         return;
      };
      if (
         tolower(img[i-2]) == 'j' &&
         tolower(img[i-1]) == 'p' &&
         tolower(img[i])   == 'g') {
         stbi_write_jpg (img, width,height, 3, rgb, 95);
         return;
      };
      if (
         tolower(img[i-2]) == 't' &&
         tolower(img[i-1]) == 'g' &&
         tolower(img[i])   == 'a') {
         stbi_write_tga(img, width,height, 3, rgb);
         return;
      };
   }
   stbi_write_bmp(img,width,height,3,rgb);
}


static inline void plot(int x, int y, int color)
{
   if (x>=0 && y>=0 && x < width && y < height)
      memcpy( rgb + y*stride +x*3, &color, 3);
}


static void draw_circleb( int CX, int CY, int R, int color)
{int a,b,c,x,y;                                          //
 x=R;                                                    //
///////////////////////////////////////////////////////////
 if(x==0)                 plot(CX  ,CY, color  );    //Bresenham
  else{ b=y=0;                                           //
   for (a=-x;a<=x;a++)    plot(CX+a,CY, color  );    //
   do{                                                   //
   a=b+1-(x<<1);                                         //
   c=a+1+(y<<1);                                         //
   b+=c-a;                                               //
   if (abs(a)<b && abs(a)<abs(c))   {x--;b=a;}           //
     else {if (b<abs(a) && b<abs(c)) y++;                //
             else               {x--;y++;b=c;}           //
     for (a=-x;a<=x;a++) {plot(CX+a,CY+y, color);    //
                          plot(CX+a,CY-y, color);    //
                         }                               //
          }                                              //
     }                                                   //
   while(y<=R);                                          //
   }                                                     //
///////////////////////////////////////////////////////////
 }




int main(int argc, char ** argv) {

   if (argc < 3) {
      printf("usage: <in_image> <out_image>\n");
      return 0;
   }
   load_rgb(argv[1]);
   draw_circleb(width/2-50,height/2+50,width/8,make_rgb(255,0,0));
   draw_circleb(width/2+50,height/2+50,width/8,make_rgb(0,255,0));
   draw_circleb(width/2,height/2-50,width/8,make_rgb(0,0,255));

   save_rgb(argv[2]);

   //gcc -Ofast rgb_sample.c -o rgb_sample.exe

}