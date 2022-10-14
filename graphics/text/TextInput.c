char *curString = 0;
int charMax = 16;
int curChar = 0;
Text *curText = 0;
UI *drawBox = 0;
void (*keyFunction)(GLFWwindow*, int, int, int, int);
char **outputAddress;

void textInput(bool active, char **output) {
	Screen *s = getWindow();
	if (active) {
		if (curString != 0) {
			free(curString);
		}
		if (curText == 0) {
			// make some sort of calculation to make sure that the text is centered, and teh right font size to not leave the text box
			curText = makeText("", 0.5, false, 0, 0, 0, 0.75);
			curText->xOffset = -0.13;
			curText->yOffset = -0.01;
			drawBox = makeUI("resources/textBox.png", 0, 1, 1);
			setScale(drawBox->a, 200, 100);
			/*
	Camera *cam = getDefaultCam();
	float xSize = 2.0 / cam->frameX;
	float ySize = 2.0 / cam->frameY;
			setScale(drawBox->a, xSize, ySize);
*/
			placeUI(drawBox, 0, 0);
			addText(drawBox, curText);
		}
		curString = (char *)calloc(sizeof(char), charMax + 1);
		curChar = 0;
		glfwSetCharCallback(s->window, textInputCallback);
		keyFunction = glfwSetKeyCallback(s->window, inputKeyFunction);
		outputAddress = output;
	} else {
		glfwSetCharCallback(s->window, 0);
		glfwSetKeyCallback(s->window, keyFunction);
	}
}

char *getString() {
	printf("%s\n", curString);
	*outputAddress = (char *)calloc(sizeof(char), strlen(curString)+1);
	strcpy(*outputAddress, curString);
	free(curString);
	curString = 0;
	return *outputAddress;
}

void textInputCallback(GLFWwindow *window, unsigned int codepoint) {
	if (curChar < charMax) {
		curString[curChar++] = codepoint;
		updateText();
		//printf("%s\n", updateStr);
		if (curChar == charMax) {
			//endInput();
		}
	}
}

void updateText() {
	char updateStr[curChar];
	strcpy(updateStr, curString);
	updateStr[curChar] = '\0';
	setTextText(curText, updateStr);
}

void inputKeyFunction(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		printf("key:%i\n", key);
		if (key == 257) {//enter
			endInput();
		} else if (key == 259) {
			curString[--curChar] = 0;
			updateText();
		}
	}
}

void renderTextInput() {
	if (curText != 0) {
		Screen *s = getWindow();
		drawUI(drawBox);
		//drawText(curText, (s->width/2) - s->width/4 + s->width*0.05, s->height/2);
	}
}

void endInput() {
	curString[curChar] = '\0';
	textInput(false, 0);
	getString();
	freeText(curText);
	curText = 0;
}
