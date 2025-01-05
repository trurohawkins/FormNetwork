#ifndef TEXTINPUT
#define TEXTINPUT

void textInput(bool active, char **outputAddy);
void textInputCallback(GLFWwindow *winbdow, unsigned int codepoint);
void inputKeyFunction(GLFWwindow *window, int key, int scancode, int action, int mods);
void endInput();
void updateText();
void renderTextInput();
#endif
