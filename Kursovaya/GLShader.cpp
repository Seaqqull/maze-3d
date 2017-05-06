#include "GLShader.h"



GLShader::GLShader()
{
	m_shaderID = m_shaderType = 0;
	m_programID = NULL;
}

GLShader::GLShader(GLuint shaderType, GLuint* programID, CONST GLchar* shaderText) {
	m_shaderText = shaderText;
	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		m_shaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_shaderText == NULL) {
			m_shaderText = {
				//"uniform float factor;                       \n" // data in all shaders
				"attribute vec3 in_vertex;                   \n"
				"varying vec3 intp_color;                    \n"
				"attribute vec3 in_color;                    \n"
				"void main () {                                     \n"
				"intp_color = in_color;                             \n"
				"gl_Position =  gl_ModelViewProjectionMatrix * vec4(in_vertex, 1.0);   \n"
				" }                                                 \n"
			};
		}
		break;
	case GL_FRAGMENT_SHADER:
		m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_shaderText == NULL) {
			m_shaderText = {
				//"uniform float factor;                             \n" // data in all shaders
				"varying vec3 intp_color;                          \n"
				"void main () {                                    \n"
				"gl_FragColor =  vec4(intp_color, 1.0);            \n"
				" }                                                \n"
			};
		}
		break;
	default:
		break;
	}	

	m_shaderType = shaderType;
	m_programID = programID;

	glShaderSource(m_shaderID, 1, &m_shaderText, NULL);
	glCompileShader(m_shaderID);
	PrintLog(m_shaderID);

	//// Shaders link to Program
	
	//glLinkProgram(*m_programID);
	//PrintLog(*m_programID);

	///*glUseProgram(ProgramId);*/

	//// ! Проверяем статус сборки
	//int link_ok;
	//glGetProgramiv(*m_programID, GL_LINK_STATUS, &link_ok);
	//if (!link_ok)
	//{
	//	std::cout << "error attach shaders \n";
	//	return;
	//}
}

void GLShader::LinkShader() {
	glAttachShader(*m_programID, m_shaderID);
}

void GLShader::LinkShader(GLuint* programID) {
	m_programID = programID;
	glAttachShader(*m_programID, m_shaderID);
}
//-------------------------------------------------------------------------
//  Printing log of errors
//-------------------------------------------------------------------------
void GLShader::PrintLog(GLuint obj)
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

GLShader::~GLShader()
{
}
