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

static void mainloop(void);

int playerid;
int playermx = 0;
int playermy = 0;

void fatal(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	fprintf(stderr, "%s: ", PACKAGE_NAME);
	vfprintf(stderr, fmt, ap);
	exit(EXIT_FAILURE);
}

static void mainloop(void)
{
	SDL_Event ev;
	uint time;
	uint dtime;

	time = SDL_GetTicks();

	for(;;) {
		while(SDL_PollEvent(&ev))
			if(handlevideo[ev.type] != NULL)
				handlevideo[ev.type](&ev);
		dtime = SDL_GetTicks();
		while(dtime - time > 5) {
			time += 5;
			gamestep();
		}
		draw();
		
		SDL_Delay(20);
	}
}

void test(void)
{
	int id;
	thing *t;

	id = thing_create(50, 50, 30, 70, draw_square);
	t = thing_get(id);
	t->dx = 0.1;
	playerid = thing_create(100, 50, 25, 25, draw_square);
}

int main(int argc, char *argv[])
{
	initvideo();
	test();
	mainloop();
}
