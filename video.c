#include <SDL/SDL.h>
#include <cairo/cairo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config.h"
#include "draw.h"
#include "main.h"
#include "video.h"

static void setvideo(void);

static void handlekey(SDL_Event *ev);
static void handlequit(SDL_Event *ev);
static void handleresize(SDL_Event *ev);

SDL_Surface *window;
SDL_Surface *canvas = NULL;
cairo_surface_t *cairosurface;
int uiwidth = 640;
int uiheight = 480;
static int sdlvideoflags = SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF;

void (*handlevideo[256])(SDL_Event *ev) = {
	[SDL_KEYDOWN] = handlekey,
	[SDL_QUIT] = handlequit,
	[SDL_VIDEORESIZE] = handleresize,
};

void initvideo(void)
{
	int err;

	err = SDL_Init(SDL_INIT_VIDEO);
	if(err)
		fatal("SDL_Init: %s.\n", SDL_GetError());
	atexit(SDL_Quit);

	setvideo();
}

static void setvideo(void)
{
	window = SDL_SetVideoMode(uiwidth, uiheight, 32, sdlvideoflags);
	if(window == NULL)
		fatal("SDL_SetVideoMode: %s.\n", SDL_GetError());

	if(canvas != NULL) {
		SDL_FreeSurface(canvas);
		cairo_surface_destroy(cairosurface);
	}
	canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, uiwidth, uiheight, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0x00000000);
	if(canvas == NULL)
		fatal("SDL_CreateRGBSurface: %s.\n", SDL_GetError());

	cairosurface = cairo_image_surface_create_for_data(
		canvas->pixels,
		CAIRO_FORMAT_RGB24,
		uiwidth, uiheight,
		canvas->pitch);
}

static void handlequit(SDL_Event *ev)
{
	exit(EXIT_SUCCESS);
}

static void handlekey(SDL_Event *ev)
{
	return;
}

static void handleresize(SDL_Event *ev)
{
	uiwidth = ev->resize.w;
	uiheight = ev->resize.h;
	setvideo();
}
