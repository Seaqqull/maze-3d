#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <ctime>

#include <GL\glew.h>
#include <GL\glut.h>

#include "Math_3d.h"
#include "GLWizard.h"
#include "GLBox.h"
#include "array.h"

#define EMPTY 0
#define WALL 1
#define VISITED 2
#define CAMERA_DISTANSE 25
#define CHANCE_GEN_KEY 10

using namespace std;
typedef unsigned int ui;
enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

class GLLabyrinth
{
private:
	// Lab generation
	int m_neighboursSize, m_randNum;

	GLuint * m_field,* m_currentCell, *m_currentNeighbour;
	GLuint ** m_neighbours;
	
	std::vector<GLuint*> m_stackCells;
	// Lab prop
	int m_begPosX, m_begPosY, m_begPosZ,
		m_width, m_height, 
		m_countWalls, m_wallShift;
	
	// Lab Data Wall
	GLfloat m_sizeWallX, m_sizeWallY, m_sizeWallZ;//5
	//GLfloat m_colorWallR, m_colorWallG, m_colorWallB;
	Vector3f m_colorWall, m_colorWallLines;
	GLuint * m_wallVBO;
	GLuint m_wallCBO, m_wallCBOLines, m_wallIBO, m_wallIBOLines;
	// Lab Data FLoor
	GLfloat m_sizeFloorY;
	//GLfloat m_colorFloorR, m_colorFloorG, m_colorFloorB;
	Vector3f m_colorFloor;
	GLuint m_floorVBO, m_floorCBO;
	// Shaders
	GLuint m_attributeVertex, m_attributeColor;
	//GLuint m_boxIBO;

	// Wizard
	GLuint m_posWizardI, m_posWizardJ;
	Vector3f m_wizardLocation;
	// Light
	Vector3f m_lightPosition;
	// Key & Exit
	GLuint m_counterGotedKeys;
	Project::array<GLBox*> m_keyArray;
public:
	GLWizard* m_Wizard;
	// Box
	GLBox* m_exitBox;
private:
	void pm_fillLabyrinth();
	void pm_getNeighbours();

	void pm_removeWall();
	void pm_setLastCell();

	int  pm_unvisitedCount();

	void pm_genWallVBO();
	void pm_genFloorVBO();

	Vector3f* pm_getVertexCube(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz);

	void pm_genCBO(GLuint * currentCBO, Vector3f currentColor, bool isMotley = false);
	Vector3f* pm_genColorCBO(Vector3f currentColor, bool isMotley = false);

	

	GLuint pm_getCellType(GLint i, GLint j);

	void pm_isThereKey(GLuint i, GLuint j);
	bool pm_genChanceKey();
	void pm_genKeys();
	void pm_clearKeys();
public:
	GLLabyrinth();
	GLLabyrinth(GLuint width, GLuint height);
	
	~GLLabyrinth();
	//
	void ReZero();

	// Labyrinth Setters
	void SetWidth(GLuint width) { m_width = width; }
	void SetHeight(GLuint height) { m_height = height; }

	GLuint GetWidth() { return m_width; }
	GLuint GetHeight() { return m_height; }

	void SetBeginFieldPosition(GLuint x, GLuint y, GLuint z) {
		m_begPosX = x;
		m_begPosY = y;
		m_begPosZ = z;
	}
	void SetColorWall(GLfloat colorWallR, GLfloat colorWallG, GLfloat colorWallB, bool isMotley = false) {
		m_colorWall = Vector3f(colorWallR, colorWallG, colorWallB);
		pm_genCBO(&m_wallCBO, m_colorWall, isMotley);
	}
	void SetColorWallLines(GLfloat colorWallR, GLfloat colorWallG, GLfloat colorWallB, bool isMotley = false) {
		m_colorWallLines = Vector3f(colorWallR, colorWallG, colorWallB);
		pm_genCBO(&m_wallCBOLines, m_colorWallLines, isMotley);
	}
	void SetColorFloor(GLfloat colorFloorR, GLfloat colorFloorG, GLfloat colorFloorB, bool isMotley = false) {
		m_colorFloor = Vector3f(colorFloorR, colorFloorG, colorFloorB);
		pm_genCBO(&m_floorCBO, m_colorFloor, isMotley);
	}

	void SetAttribyte(GLuint vertex, GLuint color) {
		this->m_attributeVertex = vertex;
		this->m_attributeColor = color;
	}
	/*void SetBoxIBO(GLuint inIBO) {
		this->m_boxIBO = inIBO;
	}*/

	//GLuint* GetPosInLabyrinth(GLuint x, GLuint y, GLuint z);
	Vector3f GetPosInLabyrinth(GLuint i, GLuint j) { return Vector3f(i * m_sizeWallX + m_sizeWallX / 2, 0, j * m_sizeWallZ + m_sizeWallZ / 2); }
	GLuint GetPosI(GLfloat z) { return floor(z / m_sizeWallZ); }
	GLuint GetPosJ(GLfloat x) { return floor(x / m_sizeWallX); }

	GLfloat GetSizeX() { return m_sizeWallX; }
	GLfloat GetSizeY() { return m_sizeWallY; }
	GLfloat GetSizeZ() { return m_sizeWallZ; }

	GLuint GetOverallKeys() { return m_keyArray.GetCount(); }
	GLuint GetCurrentKeys() { return m_counterGotedKeys; }
	// Labyrinth Gen 
	void GenerateLabyrinth();
	void GenerateLabyrinth(GLuint width, GLuint height);
	// Labyrinth Show 
	void ShowLabyrinthConsole();
	void ShowLabyrinthGL(/*GLuint vertexID, GLuint fragmentID*/);
	// Wizard
	// Checker
	void CheckLabyrinthAnimation();
	bool GetConfirmationMoveWizard(WizardView inView);
	bool GetEndGame();



















	/// Console

	friend void SetColor(ui textcolor, ui bkcolor)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
			(WORD)((bkcolor << 4) | textcolor));
	}

	void Clear() {
		system("cls");
	}

	void GotoXY(ui X, ui Y)
	{
		COORD coord = { X, Y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	ui GetCurrentY()
	{
		CONSOLE_SCREEN_BUFFER_INFO c;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
			return 0;
		return c.dwCursorPosition.Y;
	}
	ui GetRows()
	{
		CONSOLE_SCREEN_BUFFER_INFO c;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
			return 0;
		return c.dwSize.Y;
	}
	ui GetCols()
	{
		CONSOLE_SCREEN_BUFFER_INFO c;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
			return 0;
		return c.dwSize.X;
	}
	ui GetCurrentX() {
		CONSOLE_SCREEN_BUFFER_INFO c;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c))
			return 0;
		return c.dwCursorPosition.X;
	}
	void SetSize(ui X, ui Y)
	{
		COORD coord = { X, Y };
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	void CursorVisible(bool action)
	{
		HANDLE handle;
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO structCursorInfo;
		GetConsoleCursorInfo(handle, &structCursorInfo);
		structCursorInfo.bVisible = action;
		SetConsoleCursorInfo(handle, &structCursorInfo);
	}
};


static GLuint m_IBO[] = {
	1, 0, 2,
	2, 0, 3,

	4, 0, 5,
	5, 0, 1,

	6, 7, 5,
	5, 7, 4,

	2, 3, 6,
	6, 3, 7,

	5, 1, 6,
	6, 1, 2,

	3, 0, 7,
	7, 0, 4 };

static GLuint m_IBO_LINES[] = {
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	
	4, 5,
	5, 6,
	6, 7,
	7, 4,

	0, 4,
	1, 5,
	2, 6,
	3, 7};


static GLfloat m_CBO[] = {
	0.0, 0.0, 1.0,

	0.6, 0.98, 0.6,

	1.0, 0.84, 0.8,

	0.8, 0.36, 0.36,

	1.0, 0.27, 0.0,

	0.82, 0.13, 0.56,

	0.54, 0.17, 0.89,

	0.0, 1.0, 1.0 };