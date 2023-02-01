#pragma once
#include <glad/glad.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

class ShaderProgram
{
public:
	unsigned int ID;
	ShaderProgram(const char* vertex,const char* fragment);
	~ShaderProgram();
	void use();
	void setBool(const string &name, bool value)const;
	void setInt(const string &name, int value)const;
	void setFloat(const string &name, float value)const;
	void setVec3(const string &name, float v1, float v2, float v3)const;
	void setMat4(const string & name, int count, int t, float * value)const;
};

