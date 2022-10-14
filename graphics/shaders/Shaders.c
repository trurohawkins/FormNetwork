#include "Shaders.h"

GLuint makeShaderProgramFile(char *vs, char *fs) {
	const char *vertShade = NULL;
	const char *fragShade = NULL;
	int notOpened = 0;
	char *v;
	int fd = open(vs, O_RDONLY);
	if (fd != -1) {
		v = fileRec(fd, 0);
		vertShade = (const char *)v;
		close(fd);
	} else {
		printf("couldn't open file %s\n", vs);
		notOpened++;
	}
	char *f;
	fd = open(fs, O_RDONLY);
	if (fd != -1) {
		f = fileRec(fd, 0);
		fragShade = (const char *)f;
		close(fd);
	} else {
		printf("couldn't open file %s\n", fs);
		notOpened++;
	}
	printf("made shaders and v and f\n");
	GLuint program = 0;
	if (notOpened == 0) {
		program =  makeShaderProgram(vertShade, fragShade);
	}
	free(v);
	free(f);
	printf("v and f freed\n");
	return program;
}

GLuint makeShaderProgram(const char *vertShade, const char *fragShade) {
	GLuint shaderProgram = 0;
	// compile vertex and fragment shaders
	GLuint vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertShade, NULL);
	//glCompileShader(vert);
	GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragShade, NULL);
	if (compileShader(vert) && compileShader(frag)) {
		//combine shaders into executable GPU shader programme
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, frag);
		glAttachShader(shaderProgram, vert);
		glLinkProgram(shaderProgram);
		// check if link was successful
		int params = -1;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &params);
		if (GL_TRUE != params) {
			fprintf(stderr, "ERROR: could not link shader program GL index %u\n", shaderProgram);
			printProgramInfoLog(shaderProgram);
			return 0;
		}
	}
	return shaderProgram;
}

bool compileShader(GLuint shader) {
	glCompileShader(shader);
	//check for compile errors
	int params = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", shader);
		printShaderLog(shader);
		return false;
	}
	return true;
}

void printShaderLog(GLuint shader) {
		int maxLen = 2048;
		int actualLen = 0;
		char shaderLog[maxLen];
		glGetShaderInfoLog(shader, maxLen, &actualLen, shaderLog);
		printf("shader info log for GL index %u:\n%s\n", shader, shaderLog);
}

void printProgramInfoLog(GLuint program) {
	int maxLen = 2048;
	int actualLen = 0;
	char programLog[maxLen];
	glGetProgramInfoLog(program, maxLen, &actualLen, programLog);
	printf("program infor log for GL index %u:\n%s\n", program, programLog);
}

//intensive dont call for release
bool isValid(GLuint program) {//shader program index
	glValidateProgram(program);
	int params = -1;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
	printf("program %i GL_VALIDATE_STATUS = %i\n", program, params);
	if (GL_TRUE != params) {
		printProgramInfoLog(program);
		return false;
	}
	return true;
}

const char *readFile(char *file) {
	char *a;
	printf("reading file\n");
	int fd = open(file, O_RDONLY);
	if (fd != -1) {
		a = fileRec(fd, 0);
		const char *string = (const char*)a;
		close(fd);
		return string;
	} else {
		return NULL;
	}
}

char *fileRec(int fd, int cCount) {
	char c;
	char *tmp;
	if (read(fd, &c, 1) != 0) {
		tmp = fileRec(fd, cCount + 1);
		tmp[cCount] = c;
		return tmp;
	} else {
		tmp = (char*)calloc(cCount+1, sizeof(char));
		tmp[cCount] = '\0';
		return tmp;
	}
}
