#ifndef SHAD
#define SHAD
#include<unistd.h>
#include <fcntl.h>
#include<stdlib.h>
#include<stdbool.h>
#include <stdio.h>
#include "../glfwMain.h"
#endif

GLuint makeShaderProgramFile(char *vs, char *fs);
GLuint makeShaderProgram(const char *vertShade, const char *fragShade);
bool compileShader(GLuint shader);
void printShaderLog(GLuint shader);
void printProgramInfoLog(GLuint program);
bool isValid(GLuint program);//intensive dont call for release build
const char *readFile(char *file);
char *fileRec(int fd, int cCount);
