#include "ShaderHelper.h"
#include <stdio.h>

GLint compileShader(const char* path, GLuint type)
{
	char* buffer = 0;
	GLint length;
	FILE* f;
	fopen_s(&f, path, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = new char[length];
		if (buffer)
		{
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}
	else {
		puts("Error opening shader file");
		return 0;
	}

	GLint res;
	GLint shader = glCreateShader(type);
	glShaderSource(shader, 1, &buffer, &length);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);

	if (!res) {
		printf("Error compilig shader %s!\n", path);

		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar* err = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, err);

		printf(err);

		delete[] buffer;
		return 0;
	}
	delete[] buffer;
	return shader;
}