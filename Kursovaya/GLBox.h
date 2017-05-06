#pragma once
#define ANGLE_PER_SECOND 60

#include <Windows.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Math_3d.h"

class GLBox
{
private:
	GLfloat m_startingX, m_startingY, m_startingZ,
		m_startingAngleX, m_startingAngleY, m_startingAngleZ;
	GLfloat m_height, m_width;
	Vector3f m_color, m_colorLines;
	// VBO
	GLuint m_VBO;
	// CBO
	GLuint m_CBO, m_CBOLines;
	// Light
	Vector3f m_lightPosition;
	
	// Shown
	bool isShown = true;
private:
	Vector3f* pm_getVertexBox(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz);
	void pm_genCBO(GLuint * currentCBO, Vector3f currentColor);
	Vector3f* pm_genColorCBO(Vector3f currentColor);
	void pm_genBodyVBO();
	void pm_executeBox();
public:
	// Labyrinth cell size
	static GLfloat m_sizeWallX, m_sizeWallZ;
	// Shaders
	static GLuint m_attributeVertex, m_attributeColor;
	// IBO
	static GLuint m_IBO, m_IBOLines;


	GLBox();
	GLBox(GLfloat width, GLfloat height, GLfloat startingX = 0, GLfloat startingY = 0, GLfloat startingZ = 0, GLfloat startingAngleX = 0, GLfloat startingAngleY = 0, GLfloat startingAngleZ = 0);
	~GLBox();

	static void SetLabyrinthSizes(GLfloat x, GLfloat z) { m_sizeWallX = x; m_sizeWallZ = z; }

	static void SetAttribyte(GLuint vertex, GLuint color) {
		m_attributeVertex = vertex;
		m_attributeColor = color;
	}

	void SetWidth(GLfloat sizeWidth) {
		this->m_width = sizeWidth;
	}
	void SetHeight(GLfloat sizeHeight) {
		this->m_height = sizeHeight;
	}
	void SetBeginFieldPosition(GLfloat x, GLfloat y, GLfloat z) {
		m_startingX = x;
		m_startingY = y;
		m_startingZ = z;
	}
	void SetFieldPosition(GLuint i, GLuint j) { // Check
		m_startingX = j * m_sizeWallX + m_sizeWallX / 2;
		m_startingZ = i * m_sizeWallZ + m_sizeWallZ / 2;
		m_startingY = 0.2f;
	}

	static void SetIBO(GLuint inIBO) { m_IBO = inIBO; }
	static void SetIBOLines(GLuint inIBO) { m_IBOLines = inIBO; }
	void SetColor(GLfloat r, GLfloat g, GLfloat b) {
		this->m_color = Vector3f(r, g, b);
		pm_genCBO(&m_CBO, this->m_color);
	}
	void SetColorWallLines(GLfloat colorWallR, GLfloat colorWallG, GLfloat colorWallB) {
		this->m_colorLines = Vector3f(colorWallR, colorWallG, colorWallB);
		pm_genCBO(&m_CBOLines, m_colorLines);
	}
	void SetVisible(bool flag) { isShown = flag; }

	GLfloat GetPosX() { return m_startingX; }
	GLfloat GetPosZ() { return m_startingZ; }
	bool GetVisible() { return isShown; }

	void CreateBox();
	void DrawBox();
};

