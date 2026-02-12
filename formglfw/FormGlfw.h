#ifndef FORMGLFW
#define FORMGLFW
#include "../FormNetwork.h"
#include "../Form.h"
#include "../helper.h"
#include "../glfw.h"
#include "../AudioMan.h"

#include "Player.h"
#include "WorldView.h"
#include "god.h"
#include "DrawWorld.h"

void initFormGlfw();
void createWorld(int worldX, int worldY, int frameX, int frameY);
void FormLoop(void (game)(void));
void stopLoop();
void togglePause();
void toggleFPS();
void setBackgroundColor(float r, float g, float b, float a);
void setAnim(Form *f, Anim *a);
void changeSprites(Form *f, int index);
void setOffsets(Form *f, float x, float y);
void setInverts(Form *f, int axis, bool flipped);
void setRotos(Form *f, int roto);
float deltaTime();

void exitGame();

TileSet *newTileSet(char *sheet, int rows, int cols, int id);
#endif
