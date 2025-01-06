#ifndef PLAYMAN
#define PLAYMAN
typedef struct PlayerManager {
	linkedList *playerList;
} PlayerManager;

void makePlayerManager();
Player *checkPlayer(int num);
Player *addPlayer(Player *p);
void removePlayer(Player *p);
void processKeys(bool paused);
void freePlayerManager();
#endif
