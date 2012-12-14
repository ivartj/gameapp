#ifndef VIDEO_H
#define VIDEO_H

#include <SDL/SDL.h>

void initvideo(void);
extern void (*handlevideo[])(SDL_Event *ev);

extern int uiwidth;
extern int uiheight;
extern SDL_Surface *window;
extern SDL_Surface *canvas;
extern cairo_surface_t *cairosurface;

#endif
