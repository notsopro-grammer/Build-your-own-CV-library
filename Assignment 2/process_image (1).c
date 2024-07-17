#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x < 0) x = 0;
    if (x >= im.w) x = im.w - 1;
    if (y < 0) y = 0;
    if (y >= im.h) y = im.h - 1;
    return im.data[x + im.w * y + im.w * im.h * c];             
}

void set_pixel(image im, int x, int y, int c, float v)
{
    im.data[x + im.w * y + im.w * im.h * c] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float)); 
    //by loops(not sure tho)
    /*
    for(int i=0;i<im.c;i++){
    for(int j=0;j<im.h;j++){
    for(int k=0;k<im.w;k++){
    float pval=im.data[k+j*im.w+i*im.h*im.w];
    copy.data[k+j*im.w+i*im.h*im.w]=pval;
    return copy;
    }
    }
    }

    */
   return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
       for(int i=0;i<im.h;i++){
        for(int j=0;j<im.w;j++){
            float r=im.data[i*im.w+j];
            float g=im.data[i*im.w+j+im.w*im.h];
            float b=im.data[i*im.w+j+2*im.w*im.h];
            float y=0.299*r+0.587*g+0.114*b;
            gray.data[i*im.w+j]=y;
        }
    } 
    return gray;
}

void shift_image(image im, int c, float v)
{
 for(int i=0;i<im.h;i++){
     for(int j=0;j<im.w;j++){
         im.data[j+im.w*i+im.h*im.w*c]+=v;
     }
   }
}

void clamp_image(image im)
{
  for(int i=0;i<im.c;i++){
   for(int j=0;j<im.h;j++){
     for(int k=0;k<im.w;k++){
       if(im.data[i*im.w*im.h+j*im.w+k]>1)
          im.data[i*im.w*im.h+j*im.w+k]=1;
       else if(im.data[i*im.w*im.h+j*im.w+k]<0)
          im.data[i*im.w*im.h+j*im.w+k]=0;
     }
   }
  }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
   for(int i=0;i<im.h;i++){
       for(int j=0;j<im.w;j++){
           float r,g,b,V,m,C;
           r=im.data[j+im.w*i];
           g=im.data[j+im.w*i+im.w*im.h];
           b=im.data[j+im.w*i+2*im.w*im.h];
           V=three_way_max(r,g,b);
           m=three_way_min(r,g,b);
           C=V-m;
           float S;
            if(V==0)
                S=0;
            else
                S=C/V;
            float H1=0,H;
            if(C==0)
                H1=0;
            else if(V==r)
                H1=(g-b)/C;
            else if(V==g)
                H1=(b-r)/C+2;
            else if(V==b)
                H1=(r-g)/C+4;
            
            if(H1<0)
                H=H1/6+1;
            else
                H=H1/6;
           if(H<0)
                H+=1;
           else if(H>1)
                H-=1;

            im.data[j+i*im.w]=H;
            im.data[j+i*im.w+im.w*im.h]=S;
            im.data[j+i*im.w+2*im.w*im.h]=V;
        }
   }
}

void hsv_to_rgb(image im)
{
   for(int i=0;i<im.h;i++){
       for(int j=0;j<im.w;j++){
           float H,S,V;
           H=im.data[j+i*im.w];
           S=im.data[j+i*im.w+im.w*im.h];
           V=im.data[j+i*im.w+im.w*im.h*2];
           float k1,k2,k3;
           k1=((int)(5+6*H))%6 + ((5+6*H)-(int)(5+6*H));      //here we first calculated integral part then added fractional part
           k2=((int)(3+6*H))%6 + ((3+6*H)-(int)(3+6*H));
           k3=((int)(1+6*H))%6 + ((1+6*H)-(int)(1+6*H));
           float f1,f2,f3;
           f1=V-V*S*three_way_max(0,0,three_way_min(k1,4-k1,1));
           f2=V-V*S*three_way_max(0,0,three_way_min(k2,4-k2,1)); 
           f3=V-V*S*three_way_max(0,0,three_way_min(k3,4-k3,1)); 
           float r,g,b;
           r=f1;
           g=f2;                      
           b=f3;
           im.data[j+i*im.w]=r;
           im.data[j+i*im.w+im.w*im.h]=g;
           im.data[j+i*im.w+2*im.w*im.h]=b;
        }
    }                      
}
    void scale_image(image im, int c, float v){
    for(int i=0;i<im.h;i++){
        for(int j=0;j<im.w;j++){
            im.data[j+i*im.w+c*im.w*im.h]*=v;
        }
    }
}


