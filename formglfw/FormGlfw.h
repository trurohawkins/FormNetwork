#ifndef FORMGLFW
#define FORMGLFW
#include "../form/Form.h"
#include "../helper/helper.h"
#include "../graphics/glfwMain.h"
#include "../audio/Sound.h"
#include "Player.h"
#include "WorldView.h"
#include "god.h"
#include "DrawWorld.h"

void initFormGlfw();
void createWorld(int worldX, int worldY, int frameX, int frameY);
void FormLoop(void (game)(void));
void stopLoop();
void setBackgroundColor(float r, float g, float b, float a);
void setAnim(Form *f, Anim *a);
void exitGame();
#endif
