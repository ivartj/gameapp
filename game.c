#include <assert.h>
#include "main.h"
#include "game.h"

static void inctblcap(void);

static thing **things = NULL;
static int thingcount = 0;
static int thingcap = 0;
static double g = 0.005;

int thing_create(int x, int y, void (*draw)(thing *))
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
	new->draw = draw;

	assert(new != NULL);
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

void gamestep(void)
{
	thing *iter = NULL;
	thing *player;

	player = thing_get(playerid);
	assert(player != NULL);
	player->dx += (double)playermx * 0.01;
	player->dy += (double)playermy * 0.01;

	while(thing_iter(&iter)) {
		iter->x += iter->dx;
		if(iter->y + iter->dy >= 300) {
			iter->y = 300 * 2 - iter->y - iter->dy;
			iter->dy = 0 < 0.15 - iter->dy ? 0 : 0.15 - iter->dy;
		} else
			iter->y += iter->dy;
		iter->y += iter->dy;
		iter->dy += g;
	}
}
