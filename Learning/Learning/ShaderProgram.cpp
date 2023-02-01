#include "ShaderProgram.h"
ShaderProgram::ShaderProgram(const char * vertex, const char * fragment)
{
	string vertexCode, fragmentCode;
	ifstream vertFile, fragFile;
	vertFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragFile.exceptions(ifstream::failbit | ifstream::badbit);
	try {
		vertFile.open(vertex);
		fragFile.open(fragment);
		stringstream verStream, fragStream;
		verStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();
		vertFile.close();
		fragFile.close();
		vertexCode = verStream.str();
		fragmentCode = fragStream.str();
	}
	catch(ifstream::failure e){
		cout << "read code file failed" << endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "VertexShader compilation failed " << infoLog << endl;
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "FragmentShader compilation failed " << infoLog << endl;
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ShaderProgram link failed " << infoLog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::use()
{
	glUseProgram(ID);
}

void ShaderProgram::setBool(const string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const string & name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const string & name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setVec3(const string & name, float v1, float v2, float v3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}

void ShaderProgram::setMat4(const string & name, int count,int t,float * value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, t, value);
}
