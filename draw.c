#include <cairo/cairo.h>
#include <math.h>

#include "draw.h"
#include "main.h"
#include "game.h"
#include "video.h"

static cairo_t *cr;

void draw(void)
{
	thing *iter = NULL;

	if(SDL_MUSTLOCK(canvas))
		SDL_LockSurface(canvas);

	cr = cairo_create(cairosurface);

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

	cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, 0, 312.5);
	cairo_line_to(cr, uiwidth, 312.5);
	cairo_stroke(cr);

	while(thing_iter(&iter)) {
		cairo_save(cr);
		if(iter->draw)
			iter->draw(iter);
		cairo_restore(cr);
	}

	cairo_destroy(cr);

	if(SDL_MUSTLOCK(canvas))
		SDL_UnlockSurface(canvas);

	SDL_BlitSurface(canvas, NULL, window, NULL);
	SDL_Flip(window);
}

void draw_square(thing *t)
{
	int w, h;

	if(t->w || t->h) {
		w = t->w;
		h = t->h;
	} else {
		w = 25;
		h = 25;
	}
	cairo_rectangle(cr, t->x - w / 2, t->y - h / 2, w, h);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_fill(cr);
}
