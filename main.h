#ifndef MAIN_H
#define MAIN_H

#include <SDL/SDL.h>
#include <cairo/cairo.h>
#include <stdint.h>

typedef uint32_t uint;

extern int width;
extern int height;
extern SDL_Surface *window;
extern SDL_Surface *canvas;
extern cairo_surface_t *cairosurface;

#endif
