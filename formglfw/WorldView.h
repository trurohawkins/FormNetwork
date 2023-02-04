#ifndef WORLDVIEW
#define WORLDVIEW
typedef struct WorldView {
	float frameX;
	float frameY;
	float frame;
	float centerX;
	float centerY;
	int scalePower;
	//int defaultFrameX;
	//int defaultFrameY;
	int buffX;
	int buffY;
	double objSX;
	double objSY;
	float frameDest;
	float cenDestX;
	float cenDestY;
	float moveSpeed;
	float zoomSpeed;
	Camera *cam;
	Anim *background;
} WorldView;

WorldView *makeWorldView(int scale);
void initWorldView(int x, int y);
void wvMakeBackground(WorldView *wv, char *image);
Anim *wvChangeBackground(WorldView *wv, Anim *bg);
void wvDrawBackground(WorldView *wv, float *matrix); 
WorldView *getDefaultView();
//void setCenter(WorldView *c, float cp[2]);
void resizeScreen();
void calculateFrameData();
void setFrame(WorldView *w, float frame);
void setCenter(WorldView *wv, float xp, float yp);
void centerCamera(WorldView *wv, float x, float y);
void followForm(Form *f);
void unFollowForm(Form *f);
void followForms(WorldView *wv);
void lerpView(WorldView *wv);
#endif
