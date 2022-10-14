#ifndef FORMGLFW
#define FORMGLFW
#include "../form/Form.h"
#include "../helper/helper.h"
#include "../graphics/glfwMain.h"
#include "Player.h"
#include "WorldView.h"
#include "god.h"
#include "DrawWorld.h"

void initFormGlfw();
void createWorld(int worldX, int worldY, int frameX, int frameY);
void setAnim(Form *f, Anim *a);
void exitGame();
#endif
