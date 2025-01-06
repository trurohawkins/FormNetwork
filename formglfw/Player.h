#ifndef PLAYER
#define PLAYER
typedef struct Player {
	void *self;
	linkedList *controls;
	int num;
	Joypad *joy;
	void(*delFunc)(void*);
	bool active;
	bool pausePlayer;//should I process controls while game is paused?
} Player;

#include "PlayerManager.h"

Player *makePlayer(void *character, int num, void (*deleteFunc)(void*));
void addControl(Player *player, char *inp, void (*n_func)(void*,float));
void makeJoyButtControl(Player *player, char butt, void (*n_func)(void*,float));
void makeJoyAxeControl(Player *player, char butt, void (*n_func)(void*,float));
bool cmpPlayer(void *p1, void *p2);
void freePlayer(Player *p);

#endif
