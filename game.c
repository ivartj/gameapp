#include "main.h"
#include "game.h"

static void inctblcap(void);

static thing **things = NULL;
static int thingcount = 0;
static int thingcap = 0;
static double g = 0.005;

int thing_create(int x, int y, int w, int h, void (*draw)(thing *))
{
	int i;
	int idx;
	thing *new;

	if(thingcount == thingcap)
		inctblcap();
	for(i = 0; i < thingcap; i++)
		if(things[i] == NULL)
			break;
	idx = i;

	new = calloc(1, sizeof(thing));
	new->id = idx;
	new->x = x;
	new->y = y;
	new->w = w;
	new->h = h;
	new->draw = draw;

	things[idx] = new;
	thingcount++;

	return idx;
}

static void inctblcap(void)
{
	int newcap;
	int i;
	newcap = thingcap * 2 + 1;
	things = realloc(things, sizeof(thing *) * newcap);
	for(i = thingcap; i < newcap; i++)
		things[i] = NULL;
	thingcap = newcap;
}

thing *thing_get(int id)
{
	if(id >= thingcap)
		return NULL;

	return things[id];
}

int thing_destroy(int id)
{
	thing *t;

	if(id >= thingcap)
		return -1;
	if(things[id] == NULL);
		return -1;

	t = things[id];
	free(t);
	things[id] = NULL;

	return 0;
}

// NB: *iter must be NULL initially.
int thing_iter(thing **iter)
{
	int i;

	if(*iter == NULL)
		i = 0;
	else
		i = (*iter)->id + 1;
	
	for(; i < thingcap; i++)
		if(things[i] != NULL)
			break;

	if(i == thingcap)
		return 0;

	*iter = things[i];

	return 1;
}

/* Returns -1 on no collision, collision time otherwise. */
static double collidetest(double time, thing *icol, thing *jcol)
{
	double newtime;
	double y;
	double dy;
	if(icol->y + icol->h / 2 > jcol->y - jcol->h / 2)
		return -1;
	if(icol->dy - jcol->dy < 0)
		return -1;
	y = icol->y + icol->h / 2 - jcol->y + jcol->h / 2;
	dy = jcol->dy - icol->dy;
	newtime = y / dy;
	if(newtime > time)
		return -1;
	if(icol->x + icol->dx * newtime - icol->w / 2 > jcol->x + jcol->dx * newtime + jcol->w / 2)
		return -1;
	if(icol->x + icol->dx * newtime + icol->w / 2 < jcol->x + jcol->dx * newtime - jcol->w / 2)
		return -1;
	return newtime;
}

static void collide(thing *icol, thing *jcol)
{
	double dy;

	dy = icol->dy;
	icol->dy = jcol->dy;
	jcol->dy = dy;
}

void gamestep(void)
{
	thing *iter = NULL;
	thing *jter = NULL;
	thing *icol, *jcol;
	thing *player;
	double steptime;
	double coltime;
	double coltesttime;

	player = thing_get(playerid);
	player->dx += (double)playermx * 0.01;
	player->dy += (double)playermy * 0.01;

	steptime = 1;

	/* Gravity */
	/*
	iter = NULL;
	while(thing_iter(&iter))
		iter->dy += g;
	*/
	
	iter = NULL;
	do {
		icol = jcol = NULL;
		coltime = steptime;
		while(thing_iter(&iter)) {
			jter = iter;
			while(thing_iter(&jter)) {
				coltesttime = collidetest(coltime, iter, jter);
				if(coltesttime >= 0 && coltesttime < coltime) {
					coltime = coltesttime;
					icol = iter;
					jcol = jter;
				}
			}
		}
		iter = NULL;
		while(thing_iter(&iter)) {
			iter->x += iter->dx * coltime;
			iter->y += iter->dy * coltime;
		}
		steptime -= coltime;
		if(icol && jcol)
			collide(icol, jcol);
	} while(steptime > 0);
}
