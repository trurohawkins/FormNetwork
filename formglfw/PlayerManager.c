#include "PlayerManager.h"
PlayerManager *PM;

void makePlayerManager() {
	PM = (PlayerManager*)calloc(1, sizeof(PlayerManager));
	PM->playerList = makeList();
}

void addPlayer(Player *p) {
	addToList(&(PM->playerList), p);
}

void removePlayer(Player *p) {
	removeFromList(&(PM->playerList), p);
}

void processKeys() {
	linkedList *cur = getCurInput();
	if (cur != NULL && cur->data != NULL) {
		while (cur != NULL) {
			inpReceived *ir = (inpReceived*)cur->data;
			char *inp = ir->input;
			linkedList *curPlayer = PM->playerList;
			
			while (curPlayer != NULL) {
				if(curPlayer->data != NULL) {
					Player *p = (Player*)(curPlayer->data);

					if(p->active) { 
						linkedList *con = p->controls;
						while (con != NULL) {
							InpMap *tmp = (InpMap*)con->data;
							char *c = tmp->input;
							if (strCompare(inp, c) == true) {
								//printf("pressed %s matches function %s\n", inp, c);
								tmp->func(p->self, ir->val);
								break;
							} 
							con = con->next;
						}
					}

				}
				curPlayer = curPlayer->next;
			}	
			
			cur = cur->next;
		}
	}
	clearInput();
}

void freePlayerManager() {
	linkedList *cur = PM->playerList;
	linkedList * next = 0;
	while (cur != NULL) {
		next = cur->next;
		if (cur->data) {
			freePlayer(cur->data);
		}
		free(cur);
		cur = next;
	}
	free(PM);
}

