#pragma once

//  Standard Input\Output C Library
#include <stdio.h>
#include <iostream>
#include <Windows.h>

#include <GL\glew.h>
#include <GL\glut.h>


class GLShader
{
private:
	GLuint	m_shaderID, m_shaderType;
	GLuint *m_programID;
	CONST GLchar * m_shaderText;
public:
	GLShader();
	GLShader(GLuint shaderType, GLuint* programID, CONST GLchar* shaderText = NULL);

	void LinkShader();
	void LinkShader(GLuint* programID);

	GLuint& GetShaderID() { return m_shaderID; }

	~GLShader();
private:
	void PrintLog(GLuint obj);
};

