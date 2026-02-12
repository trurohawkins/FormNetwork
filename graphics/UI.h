#ifndef USERINTERFACE
#define USERINTERFACE

typedef struct UI {
	float xp;
	float yp;
	float xSize;
	float ySize;
	bool xInvert;
	bool yInvert;
	int roto;
	Anim *a;
	Text *text;
} UI;

typedef struct Button {
	UI *graphics;
	void (*func)(void);
	float *textCol1;
	float *textCol2;
} Button;

typedef struct Menu {
	linkedList *buttons;
	int count;
	Button *curButt;
	float cursorX;
	float cursorY;
	int mx;
	int my;
	float keySpeed;
} Menu;

void drawGround(linkedList *ground);
UI *makeUI(char *baseFile, int numColors, int rows, int cols);
Button *makeButton(char *baseFile, int numColors, int rows, int cols, void (*func)(void));
Menu *makeMenu(float keySpeed);
void addText(UI *ui , Text *t);
void addButtonText(Button *butt, Text *t);
void addButtonSubColor(Button *butt, float r, float g, float b, float a);
void addButton(Menu *m, Button *butt);
void setMenuActive(Menu *m, bool active);
void processMenuKeys(int key, int action);
void processMenuAxes(int axe, float val);
void processMenuControllerButtons(int butt, int onoff);
void processMenuClicks(int button, int action);
void processMenuMouseMove(GLFWwindow *window, double xpos, double ypos);
void menuMovement(float xpos, float ypos);
void drawUI(UI *ui);
void moveUI(UI *ui, int xd, int yd, float xPow, float yPow);
void placeUI(UI *ui, float xp, float yp);
void freeUI(UI *ui);
void freeButton(Button *butt);
void freeMenu(Menu *m);
void initUILists();
void freeUILists();
void initBackgroundUI();
void addBackground(UI *ui); 
void removeBackground(UI *ui); 
void drawBG();
void freeBG();
void initForegroundUI();
void addForeground(UI *ui); 
void removeForeground(UI *ui); 
void drawFG();
void freeFG();
void initPauseUI();
void addPauseUI(UI *ui);
void removePauseUI(UI *ui);
void drawActiveMenu();
void updateMenu();
void freePause();
void tmpButtFunc();
void setUIOrtho(float n_scale);
#endif
