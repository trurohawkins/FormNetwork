GAME = Test
LIB = ./

CC = gcc -c -g -I/usr/include/freetype2 -I.
LC = gcc -c -g 
WC = x86_64-w64-mingw32-gcc -c -g
RELEASELIBS = -l:libglfw3.a -l:libdl.a -l:libpthread.a -l:libfreetype.a -l:libcglm.a -lm -l:libsndfile.a -l:libportaudio.a -ljack -lasound -logg -lvorbisenc -lvorbis -lopus -lFLAC -lmpg123 -lmp3lame
DEVLIBS = -lglfw -lGL -lm -ldl -lfreetype -lcglm -lsndfile -lportaudio
GLW = -lglfw3 -lopengl32 -lgdi32 -lfreetype -lsndfile -lportaudio
GF = $(LIB)formglfw/
FD = $(LIB)form/
AD = $(LIB)actor/
ACD = $(LIB)$(AD)acts/
GD = $(LIB)graphics/
ID = $(LIB)$(GD)input/
SHD = $(LIB)$(GD)shaders/
TD = $(LIB)$(GD)text/
HD = $(LIB)helper/
AU = $(LIB)audio/

$(GAME): main.c libFormGlfw.a glad.o FormNetwork.h specs.h
	gcc -g -o  $(GAME) main.c glad.o libFormGlfw.a  $(DEVLIBS)

FormNetwork.h: formglfw/FormGlfw.h libFormGlfw.a
	gcc -E -I/usr/include/freetype2 -I. $(GF)FormGlfw.h > FormNetwork.h

standalone: $(GAME)/main.c libFormGlfw.a glad.o FormNetwork.h
	gcc -o $(GAME)/$(GAME) $(GAME)/main.c glad.o libFormGlfw.a $(RELEASELIBS) 

windows: setWindows $(GAME)/main.c libFormGlfw.a glad.o FormNetwork.h
	x86_64-w64-mingw32-gcc -o $(GAME)/$(GAME) $(GAME)/main.c -static glad.o libFormGlfw.a $(GLW)

setWindows:
	$(eval CC := $(WC))

main.o: main.c
	$(CC) -Wextra -Wall main.c -lcglm

libFormGlfw.a: FormGlfw.o Form.o helper.o glfwMain.o Shaders.o Input.o Anim.o Text.o Camera.o Sound.o
	ar rs libFormGlfw.a FormGlfw.o Form.o helper.o glfwMain.o Shaders.o Input.o Anim.o Text.o Camera.o Sound.o

FormGlfw.o: $(GF)FormGlfw.c $(GF)FormGlfw.h $(GF)Player.c $(GF)Player.h  $(GF)PlayerManager.h $(GF)PlayerManager.c  $(GF)DrawWorld.c $(GF)DrawWorld.h $(GF)WorldView.c $(GF)WorldView.h $(GF)god.h $(GF)god.c
	$(CC) -I/usr/include/freetype2 $(GF)FormGlfw.c

libform.a: Form.o helper.o
	ar rs libform.a Form.o helper.o

Form.o:  $(FD)Form.c $(FD)Form.h $(FD)World.c $(FD)World.h $(AD)Action.c $(AD)Action.h $(AD)Actor.c $(AD)Actor.h   $(AD)ActorList.h $(AD)ActorList.c $(FD)Value.h $(FD)Value.c $(FD)Cell.c $(FD)Cell.h
	$(CC) $(FD)Form.c 

Sound.o: $(AU)Sound.h $(AU)Sound.c
	$(CC) $(AU)Sound.c

libglfw.a: glfwMain.o Shaders.o Input.o Anim.o Text.o Camera.o
	ar rs libglfw.a glfwMain.o Shaders.o Input.o Anim.o Text.o Camera.o

glfwMain.o: $(GD)glfwMain.c $(GD)glfwMain.h $(SHD)glslLib.c 
	$(CC) $(GD)glfwMain.c

Shaders.o: $(SHD)Shaders.c $(SHD)Shaders.h 
	$(CC) $(SHD)Shaders.c

$(SHD)glslLib.c: $(SHD)matVS.glsl $(SHD)matFS.glsl $(SHD)textureVS.glsl $(SHD)textureFS.glsl $(SHD)singleTextureVS.glsl $(SHD)singleTextureFS.glsl $(SHD)textVS.glsl $(SHD)textFS.glsl
	cd $(SHD); ./glslToC glslLib.c matVS.glsl matFS.glsl textureVS.glsl textureFS.glsl singleTextureVS.glsl singleTextureFS.glsl textVS.glsl textFS.glsl

Graphics.o: $(GD)Graphics.c $(GD)Graphics.h
	$(CC) $(GD)Graphics.c

Anim.o: $(GD)Anim.c $(GD)Anim.h $(GD)AnimList.c $(GD)AnimList.h $(GD)TextureManager.c $(GD)TextureManager.h $(GD)UI.c $(GD)UI.h $(GD)Tile.c $(GD)Tile.h
	$(CC) $(GD)Anim.c

Text.o: $(TD)Text.c $(TD)Text.h $(TD)TextInput.c $(TD)TextInput.h
	$(CC) $(TD)Text.c -l:libfreetype.a -lcglm

Input.o: $(ID)Input.c $(ID)Input.h $(ID)Joystick.c $(ID)Joystick.h
	$(CC) $(ID)Input.c

Camera.o: $(GD)Camera.c $(GD)Camera.h
	$(CC) $(GD)Camera.c

glad.o: $(GD)glad.c $(GD)glad.h $(GD)khrplatform.h
	$(CC) $(GD)glad.c -ldl

helper.o: $(HD)helper.c $(HD)helper.h $(HD)list.c $(HD)list.h $(HD)binaryWriter.h $(HD)binaryWriter.c $(HD)sortedList.h $(HD)sortedList.c
	$(CC) $(HD)helper.c

clean:
	rm -f $(GAME)
	rm -f *.o
	rm -f vgcore*

fclean:
	rm -f *.o 
	rm -f *.a
	rm -f vgcore*
	rm -f $(GAME)
	rm -f FormNetwork.h
