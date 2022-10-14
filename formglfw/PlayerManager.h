#ifndef PLAYMAN
#define PLAYMAN
typedef struct PlayerManager {
	linkedList *playerList;
} PlayerManager;

void makePlayerManager();
void addPlayer(Player *p);
void removePlayer(Player *p);
void processKeys();
void freePlayerManager();
#endif
