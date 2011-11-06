#include "shaderHelper.hpp"

int ShaderHelper::loadShader (std::string vertexPath, std::string fragmentPath)
{
	int programPointer, vertexPointer, fragmentPointer;

	programPointer = vertexPointer = fragmentPointer = 0;
	programPointer = glCreateProgram();

	char * vertSource = loadFile(vertexPath);
	vertexPointer = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexPointer, 1, (const char**)(&(vertSource)), NULL);
	glCompileShader(vertexPointer);

	if(shader_error(vertexPointer, GL_COMPILE_STATUS))
	{
		cout << "error in vertexshader" << endl;
		show_error(vertexPointer, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(vertexPointer);
		return 0;
	}
	delete [] vertSource;
	glAttachShader(programPointer, vertexPointer);


	char * fragSource = loadFile(fragmentPath);
	fragmentPointer = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentPointer, 1, (const char**)(&(fragSource)), NULL);
	glCompileShader(fragmentPointer);

	if(shader_error(fragmentPointer, GL_COMPILE_STATUS))
	{
		cout << "error in fragmentshader" << endl;
		show_error(fragmentPointer, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(fragmentPointer);
		return 0;
	}
	delete [] fragSource;
	glAttachShader(programPointer, fragmentPointer);


	glLinkProgram(programPointer);

	glUseProgram(programPointer);
	glUseProgram(0);

	cout << "loaded " << vertexPath << " and " << fragmentPath << " successfully\n";
	return programPointer;
}

//private
char * ShaderHelper::loadFile (std::string path)
{
	FILE *fd;	long len, r; char *str;
	if (!(fd = fopen(path.c_str(), "r")))
	{
		fprintf(stderr, "Can't open file '%s' for reading\n", path.c_str());
		return NULL;
	}
	fseek(fd, 0, SEEK_END); // set position pointer to file end
	len = ftell(fd); // get the count of bytes that the pointer went
	fseek(fd, 0, SEEK_SET); // reset the pointer to the beginning of the file
	if (!(str = (char*)malloc(len * sizeof(char))))
	{
		fprintf(stderr, "Can't malloc space for '%s'\n", path.c_str());
		return NULL;
	}
	r = fread(str, sizeof(char), len, fd);
	str[r - 1] = '\0'; //make sure the string ends
	fclose(fd);
	return str;
}

//private
bool ShaderHelper::shader_error (unsigned int object, unsigned int status)
{
	int ok = 0;
	if(status == GL_LINK_STATUS)
	{
		glGetProgramiv(object, status, &ok);
		return !ok;
	}
	else if(status == GL_COMPILE_STATUS)
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &ok);
		return !ok;
	}
	return true;
}

//private
void ShaderHelper::show_error (unsigned int object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	int length;
	char *out;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &length);
	out = new char[length];
	glGet__InfoLog(object, length, NULL, out);
	cout << out << endl;
	delete [] out;
}
