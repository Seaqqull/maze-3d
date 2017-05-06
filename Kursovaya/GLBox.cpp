#include "GLBox.h"



GLBox::GLBox()
{
}

GLBox::GLBox(GLfloat width, GLfloat height, GLfloat startingX, GLfloat startingY, GLfloat startingZ, GLfloat startingAngleX, GLfloat startingAngleY, GLfloat startingAngleZ) {
	SetWidth(width);
	SetHeight(height);

	m_startingX = startingX;
	m_startingY = startingY;
	m_startingZ = startingZ;

	m_startingAngleX = startingAngleX;
	m_startingAngleY = startingAngleY;
	m_startingAngleZ = startingAngleZ;
	m_lightPosition = Vector3f(3, 0, 3);
}

GLBox::~GLBox()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_CBO);
}


Vector3f* GLBox::pm_getVertexBox(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
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

void GLBox::pm_genBodyVBO() {
	Vector3f* cubeBody = pm_getVertexBox(m_startingX - m_width / 2, m_startingY, m_startingZ - m_width / 2, m_width, m_height, m_width);

	glGenBuffers(1, &m_VBO);// How many, Where store

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), cubeBody, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete cubeBody;
}

void GLBox::pm_genCBO(GLuint * currentCBO, Vector3f currentColor) {
	glDeleteBuffers(1, &(*currentCBO));
	Vector3f* colorCBO = pm_genColorCBO(currentColor);
	glGenBuffers(1, &(*currentCBO));// How many, Where store
	glBindBuffer(GL_ARRAY_BUFFER, *currentCBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), colorCBO, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete colorCBO;
}

Vector3f* GLBox::pm_genColorCBO(Vector3f currentColor) {
	Vector3f* out_cube = new Vector3f[8];
	GLuint counter = 0;
	for (int i(0); i < 8; i++)
			out_cube[i] = Vector3f(currentColor);
	return out_cube;
}

void GLBox::pm_executeBox() {
	if (isShown) {
		GLfloat light_position[] = { m_lightPosition.x, m_lightPosition.y , m_lightPosition.z , 1.0 }; // !!!!! Свет + Визард + Враги + Ключ
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // Bind type information to buffer
		glVertexAttribPointer(m_attributeVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_CBO); // Bind type information to buffer
		glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, m_CBOLines); // Bind type information to buffer
		glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOLines);

		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void GLBox::CreateBox() {
	glDeleteBuffers(1, &m_VBO);
	pm_genBodyVBO();
}

void GLBox::DrawBox() {
	glPushMatrix();
	glTranslatef(m_startingX, m_startingY, m_startingZ);
	m_startingAngleY += (GLfloat)ANGLE_PER_SECOND / 100.0f;

	if (m_startingAngleY >= 360)
		m_startingAngleY = 0;

	glRotatef(m_startingAngleX, 1, 0, 0);
	glRotatef(m_startingAngleY, 0, 1, 0);
	glRotatef(m_startingAngleZ, 0, 0, 1);
	pm_executeBox();
	glPopMatrix();
}