#ifndef GAME_H
#define GAME_H

#include <cairo/cairo.h>

typedef struct thing thing;

struct thing {
	int id;
	double x, y;
	double dx, dy;
	double w, h;
	void (*draw)(thing *);
};

int thing_create(int x, int y, void (*draw)(thing *));
thing *thing_get(int id);
int thing_destroy(int id);
int thing_iter(thing **iter);
void gamestep(void);

#endif
