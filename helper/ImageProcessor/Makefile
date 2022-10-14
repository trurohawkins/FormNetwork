RD = ../../resources/

whiteLayers: main.c
	make whiteLayers -C ../..

poopguy: whiteLayers
	./whiteLayers $(RD)poopGuySpriteSheet.png poopguy

clean:
	rm *Palette*.c

fclean:
	rm *Palette*.c
	rm whiteLayers
