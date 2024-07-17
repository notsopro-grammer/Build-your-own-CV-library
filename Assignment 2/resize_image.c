#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    int ix = (int)roundf(x);
    int iy = (int)roundf(y);
    return get_pixel(im, ix, iy, c);
}

image nn_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);
    float width_scale = (float)im.w / w;
    float height_scale = (float)im.h / h;

    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                float im_x = (i + 0.5) * width_scale - 0.5;
                float im_y = (j + 0.5) * height_scale - 0.5;
                float val = nn_interpolate(im, im_x, im_y, k);
                set_pixel(resized, i, j, k, val);
            }
        }
    }

    return make_image(1,1,1);
}



float bilinear_interpolate(image im, float x, float y, int c)
{
    int ix = (int)x;
    int iy = (int)y;
    float dx = x - ix;
    float dy = y - iy;

    float val = (1 - dx) * (1 - dy) * get_pixel(im, ix, iy, c)
              + dx * (1 - dy) * get_pixel(im, ix + 1, iy, c)
              + (1 - dx) * dy * get_pixel(im, ix, iy + 1, c)
              + dx * dy * get_pixel(im, ix + 1, iy + 1, c);

    return val;
}

image bilinear_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);
    float width_scale = (float)im.w / w;
    float height_scale = (float)im.h / h;

    for (int k = 0; k < im.c; ++k) {
        for (int j = 0; j < h; ++j) {
            for (int i = 0; i < w; ++i) {
                float im_x = (i + 0.5) * width_scale - 0.5;
                float im_y = (j + 0.5) * height_scale - 0.5;
                float val = bilinear_interpolate(im, im_x, im_y, k);
                set_pixel(resized, i, j, k, val);
            }
        }
    }

    return make_image(1,1,1);
}

