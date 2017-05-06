#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>

#include <unordered_map>

#include <GL\glew.h>
#include <GL\glut.h>

typedef std::unordered_map<GLchar*, GLuint> Mymap;

class GLShaderProgram
{
private:
	GLuint m_programID;
	Mymap m_params;
public:
	GLShaderProgram();
	~GLShaderProgram();

	GLuint& GetProgram() { return m_programID; }

	void AttachShader(GLuint* shderID);

	void PrintLog(GLuint obj);

	void checkOpenGLerror();

	void LinkProgram();

	void ConnectProgram();

	void DisconnectProgram();

	void FreeShader();

	void AttachAttribute(GLchar* param);

	void AttachUniform(GLchar* param);

	GLuint GetAttribute(GLchar* param);

	void EnableAttribute(GLchar* param);

	void DisableAttribute(GLchar* param);
	
};

