#pragma once

#define RATIO_BODY_HEIGHT 3 / 10
#define RATIO_BODY_WIDTH 4 / 10

#define RATIO_HEAD_HEIGHT 7 / 10
#define NUMBER_HEAD_EDGES 4

#define RATIO_GLASS_HEIGHT 2 / 10
#define RATIO_GLASS_WIDTH 3 / 10
#define RATIO_GLASS_HEIGHT_POSITION 4 / 10

#include <Windows.h>

#include <GL\glew.h>
#include <GL\glut.h>

#include "Math_3d.h"

enum WizardState {
	NoneState,
	Fixedly,
	Rotation,
	Moving,
	Attack
};

enum WizardView {
	NoneView,
	Up,
	Down,
	Left,
	Right
};

class GLWizard
{
private:
	// Begin
	GLfloat m_startingX, m_startingY, m_startingZ,
		m_beginAngle, m_currentAngle, m_currentPositionXZ; // current = across animation begin = end or begin animation
	// Destination rotation angle
	GLfloat m_destinationAngle;
	WizardView m_setedDestinationView;
	// Destination Translation
	GLfloat m_destinationPositionXZ;
	// Animation time
	GLuint m_beginAnimation;
	// Animation duration miliseconds
	GLuint m_timeMove, m_timeRotation, m_timeAttack;
	// Wizard Prop
	WizardState m_wizardState;
	WizardView m_wizardView;
	// Wizard Animation Destination
	WizardView m_confirmedDestinationView;
	// Wizard
	GLfloat m_wizardHeight, m_wizardWidth;
	// Head
	GLfloat m_headHeight, m_headWidth;
	Vector3f m_colorHeadDown, m_colorHeadUp;
	// Glass
	GLfloat m_glassHeight, m_glassWidth;
	Vector3f m_colorGlass;
	GLuint m_countGlass = 0;
	// Body
	GLfloat m_bodyHeight, m_bodyWidth;
	Vector3f m_colorBody;
	// VBO
	GLuint m_bodyVBO, m_headVBO, *m_glassVBO;
	// CBO
	GLuint m_bodyCBO, m_headCBO, m_glassCBO;
	// Shaders
	GLuint m_attributeVertex, m_attributeColor;
	GLuint m_boxIBO;
	// Labyrinth cell size
	GLfloat m_sizeWallX, m_sizeWallZ;
	// S
private:
	void pm_executeWizard();
	Vector3f* pm_getVertexCube(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz);
	Vector3f* pm_getVertexCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radX, GLfloat radZ, GLfloat heightY);

	void pm_genCBO(GLuint * currentCBO, Vector3f currentColor, bool isMotley = false);
	void pn_genConeCBO(GLuint * currentCBO, Vector3f colorUp, Vector3f colorDown, bool isMotley = false);
	// CBO
	Vector3f* pm_genColorCBO(Vector3f currentColor, bool isMotley = false);
	Vector3f* pm_genColorConeCBO(Vector3f colorUp, Vector3f colorDown, bool isMotley = false);
	// VBO
	void pm_genBodyVBO();
	void pm_genGlassVBO();
	void pm_genHeadVBO();
	// Checker
	bool pm_wizardRotation(GLuint currentTime);
	bool pm_wizardMoving(GLuint currentTime);
	bool pm_wizardAttack(GLuint currentTime);
	// Get destination angle
	GLfloat pm_getAngleRotation(); // relative
	GLfloat pm_getAngleRotation(WizardView inView); // fixed
	GLfloat pm_getDestinationPosition(); 
public:
	GLWizard();
	GLWizard(GLfloat width, GLfloat height, GLfloat startingX = 0, GLfloat startingY = 0, GLfloat startingZ = 0);

	~GLWizard();
	//
	void ReZero();
	//void NewWizard();
	// Set
	void setLabyrinthSizes(GLfloat x, GLfloat z) { this->m_sizeWallX = x; this->m_sizeWallZ = z; }

	void setWidthWizard(GLfloat sizeWidth) {
		this->m_wizardWidth = sizeWidth;

		this->m_headWidth = sizeWidth;
		this->m_glassWidth = sizeWidth * RATIO_GLASS_WIDTH;
		this->m_bodyWidth = sizeWidth * RATIO_BODY_WIDTH;
	}
	void setHeightWizard(GLfloat sizeHeight) {
		this->m_wizardHeight = sizeHeight;

		this->m_headHeight = sizeHeight * RATIO_HEAD_HEIGHT;
		this->m_glassHeight = sizeHeight * RATIO_GLASS_HEIGHT;
		this->m_bodyHeight = sizeHeight * RATIO_BODY_HEIGHT;
	}
	void SetBeginFieldPosition(GLfloat x, GLfloat y, GLfloat z) {
		m_startingX = x;
		m_startingY = y;
		m_startingZ = z;
	}
	void SetFieldPosition(GLuint i, GLuint j) { // Check
		m_startingX = j * m_sizeWallX + m_sizeWallX / 2;
		m_startingZ = i * m_sizeWallZ + m_sizeWallZ / 2;
	}
	Vector3f GetFieldPosition() {
		if (m_wizardView == WizardView::Right || m_wizardView == WizardView::Left)
			return Vector3f(m_startingX + m_currentPositionXZ, m_startingY, m_startingZ);
		if (m_wizardView == WizardView::Up || m_wizardView == WizardView::Down)
			return Vector3f(m_startingX, m_startingY, m_startingZ + m_currentPositionXZ);
	}

	void SetAttribyte(GLuint vertex, GLuint color) {
		this->m_attributeVertex = vertex;
		this->m_attributeColor = color;
	}
	void SetIBO(GLuint inIBO) { m_boxIBO = inIBO; }
	// VBO
	void NewWizard();
	void CreateWizard();
	// CBO
	void setColorBody(GLfloat r, GLfloat g, GLfloat b, bool isMotley = false) {
		this->m_colorBody = Vector3f(r, g, b);
		pm_genCBO(&m_bodyCBO, this->m_colorBody);
	}
	void setColorHead(GLfloat r, GLfloat g, GLfloat b, GLfloat r_, GLfloat g_, GLfloat b_, bool isMotley = false) {
		this->m_colorHeadUp = Vector3f(r, g, b); 
		this->m_colorHeadDown = Vector3f(r_, g_, b_);
		pn_genConeCBO(&m_headCBO, this->m_colorHeadUp, this->m_colorHeadDown, isMotley);
	}
	void setColorGlass(GLfloat r, GLfloat g, GLfloat b, bool isMotley = false) {
		this->m_colorGlass = Vector3f(r, g, b); 
		pm_genCBO(&m_glassCBO, this->m_colorGlass, isMotley);
	}
	// Get
	GLfloat getPositionX() { return m_startingX; }
	GLfloat getPositionZ() { return m_startingZ; }
	WizardState getWizardState() { return m_wizardState; }
	WizardView getWizardView() { return m_wizardView; }
	// Draw
	void DrawWizard(bool isIdentityMatrix = false);
	void DrawWizard(GLfloat rotateAngle, GLfloat scaleXRotate, GLfloat scaleYRotate, GLfloat scaleZRotate, bool isIdentityMatrix = false);
	void DrawWizard(GLfloat rotateAngleX, GLfloat rotateAngleY, GLfloat rotateAngleZ, bool isIdentityMatrix = false);
	// Checker
	void CheckWizardAnimation(GLuint currentTime);
	void SetWizardState(GLuint currentTime, WizardView inView = WizardView::NoneView, WizardState inState = WizardState::NoneState);
};
static Vector3f m_ConeUp;
static Vector3f m_ConeDown;

static GLfloat m_BoxCBO[] = {
	0.0, 0.0, 1.0,

	0.6, 0.98, 0.6,

	1.0, 0.84, 0.8,

	0.8, 0.36, 0.36,

	1.0, 0.27, 0.0,

	0.82, 0.13, 0.56,

	0.54, 0.17, 0.89,

	0.0, 1.0, 1.0 };