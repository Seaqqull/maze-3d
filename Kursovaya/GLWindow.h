#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define GLEW_STATIC
#define RECORDS_PER_PAGE 5

#ifdef MY_DEBUG_MODE
//  Avoid showing up the console window
#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

//  Standard Input\Output C Library
#include <stdio.h>
#include <iostream>
#include "String.h"

//  To use functions with variables arguments
#include <stdarg.h>

//  for malloc
#include <stdlib.h>

// for M_PI
#include <cmath>

// for Vectors
//#include "Math_3d.h"

// For shaders
#include "GLShaderProgram.h"
#include "GLShader.h"

// For Labyrinth
#include "GLLabyrinth.h"
#include "GLWizard.h"
#include "GLBox.h"

//  Include GLUT, OpenGL, and GLU libraries
#include <GL/glew.h>
#include <GL/Glut.h>

#include <atltime.h>
#include <fstream>

class GLWindow
{
private:
	//  define the window position on screen
	int m_windowX;
	int m_windowY;
	
	//  variables representing the window size
	int m_windowWidth;
	int m_windowHeight;
	int m_windowHandle;

	//  variable representing the window title
	String m_windowTitle;
	GLuint m_recordsFirstRec;

	//  Tells whether to display the window full screen or not
	//  Press Alt + Esc to exit a full screen.
	bool m_screenMode, m_ShowFPS, m_gamePauseMode, m_gameModeStarted, m_recordsMode;


	// Variables for shader 
	GLShaderProgram* shaderProgram;
	GLShader* vertexShader;
	GLShader*  fragmentShader;

	// VBO
	GLuint vertV, vertC, vertI;

	//  Pointer to a font style..
	//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13, 
	//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10, 
	//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
	//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
	GLvoid *m_fontStyle;

	//  The number of frames
	int m_frameCount;

	//  Number of frames per second
	float m_FPS;

	//  currentTime - previousTime is the time elapsed
	//  between every call of the Idle function
	GLuint m_timeCurrent, m_timePrevious;
	// Timer
	CTime m_timeBeginGame;
	CTimeSpan currentTime;
	// Menu Time
	CTime m_timeBeginPause;
	//
	GLuint m_menuItem;
	//  variables used for rotation
	GLfloat m_rotateX, m_rotateY, m_rotateZ;

	//
	GLLabyrinth* m_labyrinth;
	GLWizard* m_wizard;
	//
	GLBox* m_exitBox;
	//
	Vector3f m_colorInterface;
	//
	Project::array<String*> m_records;
private:
	void pm_InitWindow();
	void pm_CenterOnScreen();
	void pm_CheckCorrectnessSize(int& w, int& h);

	void pm_newGame();
	void pm_resumeGame();
	void pm_pauseGame();

	void pm_writeData();
	void pm_readData();

	// Enable\Disable Graphic smooth
	void EnableGLSmooth();
	void DisableGLSmooth();
	void EnableGLLightning();
	void DisableGLLightning();
	// FPS
	void CalculateFPS();
	void DrawFPS();
	void DrawKeys();
	void DrawTime();
	// Output information on Screen
	void Printw(float x, float y, float z, char* format, ...);
	char* SetWordw(char* format, ...);
	// Shaders
	void InitShaders();
	//void AttachShaders();
public:
	GLWindow();
	GLWindow(String windowTitle, int windowWidth, int windowHeight, bool fullScreen);

	void SetBkColor(GLuint red, GLuint green, GLuint blue);

	void OnResizeWindow(int w, int h);
	void OnKeyboardCB(unsigned char Key, int x, int y);
	void OnDisplay(void);
	void OnIdle(void);
	
	void SetMainWindow();

	int GetWindowWidth() { return m_windowWidth; }
	int GetWindowHeight() { return m_windowHeight; }
	bool GetScreenMode() { return m_screenMode; }

	void ShowMenu();
	void ShowRecords();

	~GLWindow();
};

static const GLchar* VertexShader =
{
	"varying	vec3 v;							 \n"
	"varying	vec3 n;							 \n"
	"											 \n"
	"attribute vec3 in_vertex;                   \n"
	"varying vec3 intp_color;                    \n"
	"attribute vec3 in_color;                    \n"
	"void main () {                              \n"
	"v = vec3(gl_ModelViewMatrix * gl_Vertex);							   \n"
	"n = normalize ( gl_NormalMatrix * gl_Normal );						   \n"
	"																	   \n"
	"intp_color = in_color;												   \n"
	"gl_Position =  gl_ModelViewProjectionMatrix * vec4(in_vertex, 1.0);   \n"
	" }																	   \n"

};

//v = vec3(gl_ModelViewMatrix * gl_Vertex);							   \n\
//n = normalize(gl_NormalMatrix * gl_Normal);						   \n\

//static const GLchar* VertexShader =
//{
//	"#version 330														   \n\
//	layout (location = 0) in vec3 in_position;							   \n\
//	layout (location = 1) in vec3 in_color;								   \n\
//																		   \n\
//	out	vec3 intp_color;												   \n\
//																		   \n\
//	void main () {														   \n\
//																		   \n\
//	intp_color = in_color;												   \n\
//	gl_Position =  vec4(in_position, 1.0);  \n\
//	}"
//};

static const GLchar* FragmentShader =
{
	"varying	vec3 v;								   \n"
	"varying	vec3 n;								   \n"
	"												   \n"
	"varying vec3 intp_color;                          \n"
	"void main () {                                    \n"
	"vec4 result = vec4(0.0);				   \n"
	"for (int li = 0; li < gl_MaxLights; ++li)				   \n"
	"{										   \n"
	"vec3 viewPos = gl_LightSource[li].position.w * v;					   \n"
	"vec3 l = normalize(gl_LightSource[li].position.xyz - viewPos);		   \n"
	"vec3 e = normalize(-v);										       \n"
	"vec3 r = normalize(-reflect(l, n));								   \n"
	"																	   \n"
	"vec4 Iamb = gl_FrontLightProduct[li].ambient;						   \n"
	"																	   \n"
	"vec4 Idiff = gl_FrontLightProduct[li].diffuse * max(dot(n, l), 0.0);  \n"
	"Idiff = clamp(Idiff, 0.0, 1.0);									   \n"
	"vec4 Ispec = gl_FrontLightProduct[li].specular * pow(max(dot(r, e), 0.0), gl_FrontMaterial.shininess); \n"
	"Ispec = clamp(Ispec, 0.0, 1.0);									   \n"
	"																	   \n"
	"result += Iamb + Idiff + Ispec;						               \n"
	" }																	   \n"
	"																	   \n"
	"gl_FragColor = vec4(intp_color, 1.0) * result;						   \n"
	"}																	   \n"
};

//static const GLchar* FragmentShader =
//{
//	"#version 330 core					   \n\
//	 in vec3 intp_color;				   \n\
//	 out vec4 out_color;				   \n\
//										   \n\
//	 void main() {						   \n\
//		out_color = vec4(intp_color, 1.0f);\n\
//	 }"
//};
