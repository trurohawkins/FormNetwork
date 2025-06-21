#include "dirt.h"

Form *makeDirt(int moist) {
	Form *d = makeForm(0.7, 0.3, 0.1, 1, 1);
	d->id = 1 * getRecipePow();
	int cPow = (moist / getRecipePow()) * getRecipePow();
	float mVal = moist - cPow;
	//printf("cp %f, mVal %f if: %i\n", cPow, mVal, d->id);
	addStat(d, "moisture", mVal * 0.1);
	addStat(d, "hydroK", 1);
	float tileVal = 0 + randPercent();//randpercent used as remainer to determine which tile to use for this block
	addStat(d, "tile", tileVal);//or anim
	//addToList(&(theWorld->terrain), d);
	return d; //makeForm(0.7, 0.3, 0.1, 0, 0);
}

int saveDirt(Form *d) {
	int id = d->id;
	id = clamp(id += *getStat(d, "moisture") * 10, 10, 19);
	//printf("dirt w/%f becomes %i\n", *getStat(d,"moisture"), id);
	return id;
}

Form *makeStone(int null) {
	Form *s = makeForm(0.2, 0.3, 0.4, 1, 1);
	s->id = 2 * getRecipePow();
	addStat(s, "moisture", 0);
	addStat(s, "hydroK", 1);
	addStat(s, "tile", 1);//or anim
	//addToList(&(theWorld->terrain), d);
	return s; //makeForm(0.7, 0.3, 0.1, 0, 0);
}

// make Square fun
// x,y are starting point of square, z is side length
// goes clock wise 
// STAUS: runtime error? not showing up in game world
// no complier erros (So not syntax error?)
//

void makeSquare(int x, int y, int z) {
	float moist = 0;
	Form *b = makeDirt(moist) ;
	//addToList(&(theWorld->terrain), b);
	for (int i = 1; i < z ; i++) {
		for (int j = 1; j < z ; j++) {
			placeForm( x + i, y + j, b) ;
		}
	}
}

void makeStoneSquare(int x, int y, int z) {
	//addToList(&(theWorld->terrain), b);
	for (int i = 1; i < z ; i++) {
		for (int j = 1; j < z ; j++) {
			Form *b = makeStone(0) ;
			placeForm( x + i, y + j, b) ;
		}
	}
}

 void makeCircle(int x, int y, int r) {
	//printf("making circle ");
	float moist = 0;
	Form *b = makeDirt(moist) ;
	//addToList(&(theWorld->terrain), b);
	int sx = x - r ;
	int sy = y - r ;
	int test ;
	int xd ;
	int yd ;
	//printf(" starting at (%i, %i) ", sx, sy);
	for (int i = sx; i < ( sx + (2*r)+1 ) ; i++) {
		for (int j = sy; j < ( sy + (2*r)+1 ); j++) {
		  xd = (x-i);
		  yd = (y-j);	  
		  if ( sqrt ( (xd * xd) + (yd *yd) )  <= r )  {
	   	test =  (sqrt( (xd * xd) + (yd *yd) )) ;  	
		   //printf(" d=%i ", test) ;
		    placeForm(i, j, b) ;
		    //printf(" (%i, %i) ", i, j);

}}}}

void dirtFloor(int height) {
	float moist = 0;
	Form *d = makeDirt(moist);
	//addToList(&(theWorld->terrain), d);
	int maxGrow = 6;
	for (int x = 0; x < theWorld->x; x++) {
		for(int y = 0; y < height; y++) {
			placeForm(x, y, d);
		}
		if (randPercent() > 0.75) {
			int newGrow = (int)(randPercent() * maxGrow);
			if (randPercent() > 0.5) {
				newGrow *= -1;
			}
			height = clamp(height + newGrow, 1, theWorld->y - 1);
		}
		//printf("ng: %i, h: %i\n", newGrow, height);
	}
}

