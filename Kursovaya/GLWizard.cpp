#include "GLWizard.h"



GLWizard::GLWizard()
{
}

GLWizard::GLWizard(GLfloat width, GLfloat height, GLfloat startingX, GLfloat startingY, GLfloat startingZ) {
	setWidthWizard(width);
	setHeightWizard(height);

	m_startingX = startingX;
	m_startingY = startingY;
	m_startingZ = startingZ;

	ReZero();
}

GLWizard::~GLWizard()
{
	glDeleteBuffers(1, &m_bodyVBO);
	glDeleteBuffers(1, &m_headVBO);
	glDeleteBuffers(2, m_glassVBO);

	glDeleteBuffers(1, &m_bodyCBO);
	glDeleteBuffers(1, &m_headCBO);
	glDeleteBuffers(1, &m_glassCBO);
}

void GLWizard::DrawWizard(bool isIdentityMatrix) {
	glPushMatrix();
	if (isIdentityMatrix)	glLoadIdentity();
	/*glTranslatef(m_startingX, m_startingY, m_startingZ);*/
	if (m_wizardView == WizardView::Right || m_wizardView == WizardView::Left)
		glTranslatef((m_startingX + m_currentPositionXZ), m_startingY, m_startingZ);
	if (m_wizardView == WizardView::Up || m_wizardView == WizardView::Down)
		glTranslatef(m_startingX, m_startingY, (m_startingZ + m_currentPositionXZ));
	glRotatef(m_beginAngle + m_currentAngle, 0, 1, 0);
	pm_executeWizard();
	glPopMatrix();
}
void GLWizard::DrawWizard(GLfloat rotateAngle, GLfloat scaleXRotate, GLfloat scaleYRotate, GLfloat scaleZRotate, bool isIdentityMatrix ) {
	glPushMatrix();
	if (isIdentityMatrix)	glLoadIdentity();
	glTranslatef(m_startingX, m_startingY, m_startingZ);
	glRotatef(rotateAngle, scaleXRotate, scaleYRotate, scaleZRotate);
	pm_executeWizard();
	glPopMatrix();
}
void GLWizard::DrawWizard(GLfloat rotateAngleX, GLfloat rotateAngleY, GLfloat rotateAngleZ, bool isIdentityMatrix) {
	glPushMatrix();
	if (isIdentityMatrix)	glLoadIdentity();
	glTranslatef(m_startingX, m_startingY, m_startingZ);
	glRotatef(rotateAngleX, 1, 0, 0);
	glRotatef(rotateAngleY, 0, 1, 0);
	glRotatef(rotateAngleZ, 0, 0, 1);
	pm_executeWizard();
	glPopMatrix();
}

void GLWizard::NewWizard() {
	glDeleteBuffers(1, &m_headVBO);
	glDeleteBuffers(m_countGlass, m_glassVBO);
	glDeleteBuffers(1, &m_bodyVBO);	
}

void GLWizard::CreateWizard() {
	NewWizard();
	pm_genBodyVBO();
	pm_genGlassVBO();
	pm_genHeadVBO();
}

void GLWizard::ReZero() {
	SetFieldPosition(1, 1);
	this->m_confirmedDestinationView = WizardView::NoneView;
	this->m_setedDestinationView = WizardView::NoneView;
	this->m_wizardState = WizardState::Fixedly;
	this->m_wizardView = WizardView::Left;
	this->m_beginAngle = pm_getAngleRotation(m_wizardView);
	this->m_currentAngle = 0;
	this->m_currentPositionXZ = 0;
	this->m_destinationAngle = 0;
	this->m_destinationPositionXZ = 0;
	this->m_beginAnimation = 0;
	this->m_timeMove = 500, this->m_timeRotation = 500, this->m_timeAttack = 500;
	//
	m_ConeUp = Vector3f(0, 0, 0);
	m_ConeDown = Vector3f(1, 1, 1);
}

void GLWizard::CheckWizardAnimation(GLuint currentTime) {
	switch (m_wizardState)
	{
	case WizardState::Rotation:
		if (!pm_wizardRotation(currentTime))
			m_wizardState = WizardState::Fixedly;
		break;
	case WizardState::Moving:
		if (!pm_wizardMoving(currentTime))
			m_wizardState = WizardState::Fixedly;
		break;
	case WizardState::Attack:
		if (!pm_wizardAttack(currentTime))
			m_wizardState = WizardState::Fixedly;
		break;
	default:
		break;
	}
}
void GLWizard::SetWizardState(GLuint currentTime, WizardView inView, WizardState inState) { // When key pressed
	if (m_wizardState == WizardState::Fixedly) {
		if (inState == WizardState::NoneState) { // move | rotation
			if (m_wizardView == inView) { // move
				m_beginAnimation = currentTime;
				m_wizardState = WizardState::Moving;
			}
			else { // rotation
				m_beginAnimation = currentTime;
				m_wizardState = WizardState::Rotation;
				m_setedDestinationView = inView;
			}
		}
		else { // attack
			m_beginAnimation = currentTime;
			m_wizardState = WizardState::Attack;
		}
	}
}

bool GLWizard::pm_wizardRotation(GLuint currentTime) {
	if (currentTime - m_beginAnimation <= m_timeRotation) {
		if (m_confirmedDestinationView == WizardView::NoneView) {
			m_confirmedDestinationView = m_setedDestinationView;
			m_destinationAngle = pm_getAngleRotation();
		}		
		m_currentAngle = m_destinationAngle * ((currentTime - m_beginAnimation) / (float)m_timeRotation);
		return true;
	}
	else {// при окончании анимации
		m_wizardView = m_confirmedDestinationView;
		m_beginAngle = pm_getAngleRotation(m_wizardView);// 
		m_currentAngle = m_destinationAngle = 0;
		m_confirmedDestinationView = m_setedDestinationView = WizardView::NoneView;
		return false;
	}
}
bool GLWizard::pm_wizardMoving(GLuint currentTime) {
	if (currentTime - m_beginAnimation <= m_timeMove) {
		if (m_confirmedDestinationView == WizardView::NoneView) { // for x and y
			m_confirmedDestinationView = m_wizardView;
			m_destinationPositionXZ = pm_getDestinationPosition();
		}
		m_currentPositionXZ = m_destinationPositionXZ * ((currentTime - m_beginAnimation) / (float)m_timeMove);
		return true;
	}
	else {// при окончании анимации
		m_currentPositionXZ = m_destinationPositionXZ;
		if (m_wizardView == WizardView::Right || m_wizardView == WizardView::Left)
			m_startingX += m_currentPositionXZ;
		if (m_wizardView == WizardView::Up || m_wizardView == WizardView::Down)
			m_startingZ += m_currentPositionXZ;
		m_confirmedDestinationView = WizardView::NoneView;
		m_currentPositionXZ = 0;
		return false;
	}
}
bool GLWizard::pm_wizardAttack(GLuint currentTime) {
	if (currentTime - m_beginAnimation < m_timeAttack) {

		return true;
	}
	else {// при окончании анимации
		m_confirmedDestinationView = m_setedDestinationView = WizardView::NoneView;
		return false;
	}
}

GLfloat GLWizard::pm_getAngleRotation() {
	if (m_wizardView == WizardView::Left) {
		switch (m_confirmedDestinationView)
		{
		case Up:
			return -90.0f;
		case Down:
			return 90.0f;
		case Right:
			return 180.0f;
		}
	}else if (m_wizardView == WizardView::Right) {
		switch (m_confirmedDestinationView)
		{
		case Up:
			return 90.0f;
		case Down:
			return -90.0f;
		case Left:
			return 180.0f;
		}
	}
	else if (m_wizardView == WizardView::Up) {
		switch (m_confirmedDestinationView)
		{
		case Down:
			return 180.0f;
		case Left:
			return 90.0f;
		case Right:
			return -90.0f;
		}
	}
	else if (m_wizardView == WizardView::Down) {
		switch (m_confirmedDestinationView)
		{
		case Up:
			return 180.0f;
		case Left:
			return -90.0f;
		case Right:
			return 90.0f;
		}
	}	
}

GLfloat GLWizard::pm_getAngleRotation(WizardView inView) {
	switch (inView)
	{
	case Up:
		return -90.0f;
	case Down:
		return 90.0f;
	case Left:
		return 0.0f;
	case Right:
		return 180.0f;
	}
}

GLfloat GLWizard::pm_getDestinationPosition() {
	if (m_wizardView == WizardView::Up || m_wizardView == WizardView::Down)
		return (m_wizardView == WizardView::Up) ? -m_sizeWallZ : m_sizeWallZ;
	else if(m_wizardView == WizardView::Left || m_wizardView == WizardView::Right)
		return (m_wizardView == WizardView::Right) ? m_sizeWallX : -m_sizeWallX;
}

void GLWizard::pm_executeWizard() {
	

	GLfloat light_position[] = { 3, 0 , 3, 1.0 }; // !!!!! Свет + Визард + Враги + Ключ 5 3 9  подсчёты поворота в IDLE
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glBindBuffer(GL_ARRAY_BUFFER, m_bodyVBO); // Bind type information to buffer
	glVertexAttribPointer(m_attributeVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_bodyCBO); // Bind type information to buffer
	glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_boxIBO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (int i(0); i < m_countGlass; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_glassVBO[i]); // Bind type information to buffer
		glVertexAttribPointer(m_attributeVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_glassCBO); // Bind type information to buffer
		glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_boxIBO);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_headVBO); // Bind type information to buffer
	glVertexAttribPointer(m_attributeVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_headCBO); // Bind type information to buffer
	glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, NUMBER_HEAD_EDGES * 3 * 2 * 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//delete cubeImpact;
}

Vector3f* GLWizard::pm_getVertexCube(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
	Vector3f * out_cube = new Vector3f[8];

	out_cube[0] = Vector3f(x, y, z + sz);

	out_cube[1] = Vector3f(x, y + sy, z + sz);

	out_cube[2] = Vector3f(x + sx, y + sy, z + sz);

	out_cube[3] = Vector3f(x + sx, y, z + sz);

	out_cube[4] = Vector3f(x, y, z);

	out_cube[5] = Vector3f(x, y + sy, z);

	out_cube[6] = Vector3f(x + sx, y + sy, z);

	out_cube[7] = Vector3f(x + sx, y, z);

	return out_cube;
}

Vector3f* GLWizard::pm_getVertexCone(GLfloat x, GLfloat y, GLfloat z, GLfloat radX, GLfloat radZ, GLfloat heightY) {
	GLuint counter = 6;
	Vector3f* arrOut = new Vector3f[NUMBER_HEAD_EDGES * 3 * 2];// * 2

	arrOut[0] = Vector3f(x, y, z);
	arrOut[1] = Vector3f(x + cos(0.0) * radX, y, z - sin(0.0) * radZ);
	arrOut[2] = Vector3f(x + cos(M_PI * 2 / NUMBER_HEAD_EDGES) * radX, y, z - sin(M_PI * 2 / NUMBER_HEAD_EDGES) * radZ);

	arrOut[3] = Vector3f(x, y + heightY, z);
	arrOut[4] = arrOut[1];
	arrOut[5] = arrOut[2];

	for (double i = 2 * (M_PI * 2 / NUMBER_HEAD_EDGES); i <= M_PI * 2; i += M_PI * 2 / NUMBER_HEAD_EDGES)
	{

		arrOut[counter] = Vector3f(x, y, z);
		arrOut[counter + 1] = arrOut[counter - 4];
		arrOut[counter + 2] = Vector3f(x + cos(i) * radX, y, z - sin(i) * radZ);

		arrOut[counter + 3] = arrOut[counter - 3];
		arrOut[counter + 4] = arrOut[counter + 1];
		arrOut[counter + 5] = arrOut[counter + 2];

		counter += 6;
	}

	return arrOut;
}

void GLWizard::pm_genCBO(GLuint * currentCBO, Vector3f currentColor, bool isMotley) {
	glDeleteBuffers(1, &(*currentCBO));
	Vector3f* colorCBO = pm_genColorCBO(currentColor, isMotley);
	glGenBuffers(1, &(*currentCBO));// How many, Where store
	glBindBuffer(GL_ARRAY_BUFFER, *currentCBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), colorCBO, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete colorCBO;
}

void GLWizard::pn_genConeCBO(GLuint * currentCBO, Vector3f colorUp, Vector3f colorDown, bool isMotley) {
	glDeleteBuffers(1, &(*currentCBO));
	Vector3f* colorCBO = pm_genColorConeCBO(colorUp, colorDown, isMotley);
	glGenBuffers(1, &(*currentCBO));// How many, Where store
	glBindBuffer(GL_ARRAY_BUFFER, *currentCBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * 2 * 3 * sizeof(GLfloat), colorCBO, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete colorCBO;
}

Vector3f* GLWizard::pm_genColorCBO(Vector3f currentColor, bool isMotley) {
	Vector3f* out_cube = new Vector3f[8];
	GLuint counter = 0;
	for (int i(0); i < 8; i++)
		if (isMotley)
			out_cube[i] = Vector3f(m_BoxCBO[counter++], m_BoxCBO[counter++], m_BoxCBO[counter++]);
		else
			out_cube[i] = Vector3f(currentColor);
	return out_cube;
}

Vector3f* GLWizard::pm_genColorConeCBO(Vector3f colorUp, Vector3f colorDown, bool isMotley) {
	Vector3f* out_cone = new Vector3f[NUMBER_HEAD_EDGES * 3 * 2];
	GLuint counter = 0;
	for (int i(0); i < NUMBER_HEAD_EDGES * 3 * 2; i += 6) {
		if (isMotley) {
			out_cone[i] = m_ConeDown;
			out_cone[i + 1] = m_ConeDown;
			out_cone[i + 2] = m_ConeDown;

			out_cone[i + 3] =  m_ConeUp;
			out_cone[i + 4] =  m_ConeDown;
			out_cone[i + 5] =  m_ConeDown;
		}
		else {
			out_cone[i] = colorDown;
			out_cone[i + 1] = colorDown;
			out_cone[i + 2] = colorDown;

			out_cone[i + 3] = colorUp;
			out_cone[i + 4] = colorDown;
			out_cone[i + 5] = colorDown;
		}
	}
	return out_cone;
}

void GLWizard::pm_genBodyVBO() {
	Vector3f* cubeBody = pm_getVertexCube(m_startingX - m_bodyWidth / 2, m_startingY, m_startingZ - m_bodyWidth / 2, m_bodyWidth, m_bodyHeight, m_bodyWidth);

	glGenBuffers(1, &m_bodyVBO);// How many, Where store

	glBindBuffer(GL_ARRAY_BUFFER, m_bodyVBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), cubeBody, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete cubeBody;
}
void GLWizard::pm_genGlassVBO() {
	m_countGlass = 2;
	m_glassVBO = new GLuint[m_countGlass];
	Vector3f* cubeLeftGlass = pm_getVertexCube(m_startingX - m_headWidth / 2 + 0.5, m_wizardHeight * RATIO_GLASS_HEIGHT_POSITION, m_startingZ + m_glassWidth / 4, 0.5, m_glassHeight, m_glassWidth);
	Vector3f* cubeRightGlass = pm_getVertexCube(m_startingX - m_headWidth / 2 + 0.5, m_wizardHeight * RATIO_GLASS_HEIGHT_POSITION, m_startingZ - m_glassWidth - m_glassWidth / 4, 0.5, m_glassHeight, m_glassWidth);

	glGenBuffers(2, m_glassVBO);// How many, Where store

	glBindBuffer(GL_ARRAY_BUFFER, m_glassVBO[0]); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), cubeLeftGlass, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_glassVBO[1]); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), cubeRightGlass, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete cubeLeftGlass;
	delete cubeRightGlass;
}
void GLWizard::pm_genHeadVBO() {
	Vector3f* cubeHead = pm_getVertexCone(m_startingX, m_startingY + m_bodyHeight, m_startingZ, m_headWidth / 2, m_headWidth / 2, m_headHeight);

	glGenBuffers(1, &m_headVBO);// How many, Where store

	glBindBuffer(GL_ARRAY_BUFFER, m_headVBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, NUMBER_HEAD_EDGES * 3 * 2 * 3 * sizeof(GLfloat), cubeHead, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete cubeHead;
}