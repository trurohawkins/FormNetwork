typedef struct AnimStamp {
	Anim *anim;
	float *poses;
	int sprite;
	int roto;
	float *color;
} AnimStamp;

typedef struct AnimOrder {
	int order;
	linkedList *stamps;
	linkedList *anims;
	linkedList *poses;
	linkedList *sprites;
	linkedList *rotations;
	linkedList *tints;
} AnimOrder;

extern AnimOrder *front;
extern AnimOrder *mid;
extern AnimOrder *back;

extern SortedList *backLayers;
extern SortedList *layers;

AnimOrder *makeAnimOrder(int order);
//default -1 to not affect sprite and rotation, check for making cure mulitples don't get added
void addAnimToOrder(int drawOrder, Anim *anim, float x, float y, int sprite, int rotation, float *tint, bool check);
void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize);
bool compareStamp(void *s1, void *s2);
void freeStamp(void *stamp);
void freeAnimOrder(AnimOrder *ao);

