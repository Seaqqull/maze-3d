#include "GLShaderProgram.h"



GLShaderProgram::GLShaderProgram()
{
	m_programID = glCreateProgram();

	GLenum ErrorCheckValue = glGetError();

	if (glCreateShader)
		printf(" ---- shader suppot ok ---\n");
	else
	{
		printf(" ---- no shader support ---\n");
		return;
	}
}
GLShaderProgram::~GLShaderProgram()
{
}

void GLShaderProgram::PrintLog(GLuint obj)
{
	int infologLength = 0;
	int maxLength;

	if (glIsShader(obj))
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &maxLength);

	char infoLog[1255];

	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

	if (infologLength > 0)
		printf("\n Error detail: %s\n", infoLog);
}
//-------------------------------------------------------------------------
// OpenGL Errors
//-------------------------------------------------------------------------
void GLShaderProgram::checkOpenGLerror()
{
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode);
}

void GLShaderProgram::AttachShader(GLuint* shderID) {
	glAttachShader(m_programID, *shderID);
}

void GLShaderProgram::LinkProgram() {
	glLinkProgram(m_programID);
	PrintLog(m_programID);

	// ! Проверяем статус сборки
	int link_ok;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		return;
	}
	else
		std::cout << " --- shaders attach ok --- \n";
}

void GLShaderProgram::ConnectProgram() {
	glUseProgram(m_programID);
}

void GLShaderProgram::DisconnectProgram() {
	glUseProgram(NULL);
}

void GLShaderProgram::FreeShader()
{
	// ! Передавая ноль, мы отключаем шейдрную программу
	glUseProgram(0);
	// ! Удаляем шейдерную программу
	glDeleteProgram(m_programID);
}

void GLShaderProgram::AttachAttribute(GLchar* param) {

	m_params.insert(Mymap::value_type(param, glGetAttribLocation(m_programID, param)));

	if (m_params.at(param) == -1)
	{
		std::cout << "could not bind attrib " << param << std::endl;
		return;
	}
	checkOpenGLerror();
}

void GLShaderProgram::AttachUniform(GLchar* param) {
	m_params.insert(Mymap::value_type(param, glGetUniformLocation(m_programID, param)));
	if (m_params.at(param) == -1)
	{
		std::cout << "could not bind uniform " << param << std::endl;
		return;
	}
	checkOpenGLerror();
}

GLuint GLShaderProgram::GetAttribute(GLchar* param) {
	return m_params.at(param);
}

void GLShaderProgram::EnableAttribute(GLchar* param) {
	glEnableVertexAttribArray(m_params.at(param));
}

void GLShaderProgram::DisableAttribute(GLchar* param) {
	glDisableVertexAttribArray(m_params.at(param));
}