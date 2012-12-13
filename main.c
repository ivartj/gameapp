#include <SDL/SDL.h>
#include <cairo/cairo.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"
#include "draw.h"
#include "main.h"

void fatal(char *fmt, ...);
void initvideo(void);
void draw(void);
void mainloop(void);
void setvideo(void);

void handlekey(SDL_Event *ev);
void handlequit(SDL_Event *ev);
void handleresize(SDL_Event *ev);

SDL_Surface *window;
SDL_Surface *canvas = NULL;
cairo_surface_t *cairosurface;
int width = 640;
int height = 480;
int sdlvideoflags = SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF;

void (*handlevideo[256])(SDL_Event *ev) = {
	[SDL_KEYDOWN] = handlekey,
	[SDL_QUIT] = handlequit,
	[SDL_VIDEORESIZE] = handleresize,
};

void fatal(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "%s: ", PACKAGE_NAME);
	vfprintf(stderr, fmt, ap);
	exit(EXIT_FAILURE);
}

void initvideo(void)
{
	int err;

	err = SDL_Init(SDL_INIT_VIDEO);
	if(err)
		fatal("SDL_Init: %s.\n", SDL_GetError());
	atexit(SDL_Quit);

	setvideo();
}

void setvideo(void)
{
	window = SDL_SetVideoMode(width, height, 32, sdlvideoflags);
	if(window == NULL)
		fatal("SDL_SetVideoMode: %s.\n", SDL_GetError());

	if(canvas == NULL) {
		SDL_FreeSurface(canvas);
		cairo_surface_destroy(cairosurface);
	}
	canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0x00000000);
	if(canvas == NULL)
		fatal("SDL_CreateRGBSurface: %s.\n", SDL_GetError());

	cairosurface = cairo_image_surface_create_for_data(
		canvas->pixels,
		CAIRO_FORMAT_RGB24,
		width, height,
		canvas->pitch);
}

void handlequit(SDL_Event *ev)
{
	exit(EXIT_SUCCESS);
}

void handlekey(SDL_Event *ev)
{
	return;
}

void handleresize(SDL_Event *ev)
{
	width = ev->resize.w;
	height = ev->resize.h;
	setvideo();
}

void mainloop(void)
{
	SDL_Event ev;
	uint time;
	uint dtime;

	time = SDL_GetTicks();

	for(;;) {
		while(SDL_PollEvent(&ev))
			if(handlevideo[ev.type])
				handlevideo[ev.type](&ev);
		dtime = SDL_GetTicks();
		while(dtime - time > 5) {
			time += 5;
			gamestep();
		}
		draw();
		
		SDL_Delay(10);
	}
}

void test(void)
{
	int id;
	thing *t;

	id = thing_create(50, 50, draw_square);
	t = thing_get(id);
	t->dx = 0.1;
}

int main(int argc, char *argv[])
{
	initvideo();
	test();
	mainloop();
}
