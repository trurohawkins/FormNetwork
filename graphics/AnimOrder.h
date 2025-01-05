typedef struct AnimOrder {
	int order;
	linkedList *anims;
	linkedList *poses;
	linkedList *sprites;
	linkedList *rotations;
} AnimOrder;

extern AnimOrder *front;
extern AnimOrder *mid;
extern AnimOrder *back;

AnimOrder *makeAnimOrder(int order);
//default -1 to not affect sprite and rotation, check for making cure mulitples don't get added
void addAnimToOrder(int drawOrder, Anim *anim, float x, float y, int sprite, int rotation, bool check);
void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize);
void freeAnimOrder(AnimOrder *ao);

