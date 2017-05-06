#include "GLLabyrinth.h"



GLLabyrinth::GLLabyrinth()
{
	m_neighbours = new GLuint*[4];
}
GLLabyrinth::GLLabyrinth(GLuint width, GLuint height) {
	srand(time(NULL));
	SetWidth(width);
	SetHeight(height);
	CursorVisible(false);
	ReZero();

	glGenBuffers(1, &m_wallIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IBO) , m_IBO, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_wallIBOLines);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallIBOLines);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IBO_LINES), m_IBO_LINES, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Wizard
	GLBox::SetIBO(m_wallIBO);
	GLBox::SetIBOLines(m_wallIBOLines);

	/*m_exitBox = new GLBox(1.2f, 3.0f);
	m_exitBox->SetColor(0.7f, 0.2f, 0.2f);
	m_exitBox->SetColorWallLines(0.0, 0.0, 0.0);
	m_exitBox->CreateBox();
	m_exitBox->SetBeginFieldPosition(7.0f, 1.0f, 7.0f);*/
	
	//m_Wizard->CreateWizard();
	//m_Wizard->
}

GLLabyrinth::~GLLabyrinth()
{
	glDeleteBuffers(m_countWalls, m_wallVBO);
	glDeleteBuffers(1, &m_wallCBO);
	glDeleteBuffers(1, &m_wallIBO);
	glDeleteBuffers(1, &m_wallIBOLines);

	glDeleteBuffers(1, &m_floorVBO);
	glDeleteBuffers(1, &m_floorCBO);
}

void GLLabyrinth::ReZero() {
	delete m_neighbours;
	delete m_field;
	glDeleteBuffers(m_countWalls, m_wallVBO);


	m_neighbours = new GLuint*[4];
	m_field = new GLuint[m_height * m_width];
	pm_fillLabyrinth();
	m_posWizardI = m_posWizardJ = 1;
	m_counterGotedKeys = 0;
	m_wallShift = 2;
	m_sizeFloorY = 0.5f;
	m_lightPosition = Vector3f(3, 0, 3);
	m_sizeWallX = 2, m_sizeWallY = 3, m_sizeWallZ = 2;
	pm_clearKeys();
}

void GLLabyrinth::pm_fillLabyrinth() {
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			if ((i % 2 != 0 && j % 2 != 0) && (i < m_height - 1 && j < m_width - 1))
				m_field[(i * m_width) + j] = EMPTY;
			else
				m_field[(i * m_width) + j] = WALL;
}

void GLLabyrinth::pm_getNeighbours() {
	m_neighboursSize = 0;
	if ((m_currentCell - m_field) % m_width > 1 && (*(m_currentCell - m_wallShift) != WALL && *(m_currentCell - m_wallShift) != VISITED)) // left
		m_neighbours[m_neighboursSize++] = m_currentCell - m_wallShift;
	if ((m_currentCell - m_field) % m_width < m_width - m_wallShift && (*(m_currentCell + m_wallShift) != WALL && *(m_currentCell + m_wallShift) != VISITED)) // right
		m_neighbours[m_neighboursSize++] = m_currentCell + m_wallShift;
	if ((m_currentCell - m_field) - m_width * m_wallShift > 0 && (*(m_currentCell - m_width * m_wallShift) != WALL && *(m_currentCell - m_width * m_wallShift) != VISITED)) // top
		m_neighbours[m_neighboursSize++] = m_currentCell - m_width * m_wallShift;
	if ((m_currentCell - m_field) + m_width * m_wallShift < m_height * m_width && (*(m_currentCell + m_width * m_wallShift) != WALL && *(m_currentCell + m_width * m_wallShift) != VISITED)) // bottom
		m_neighbours[m_neighboursSize++] = m_currentCell + m_width * m_wallShift;
}

void GLLabyrinth::pm_removeWall() {
	if (m_currentNeighbour > m_currentCell)
		if (m_currentNeighbour - m_currentCell == m_width * m_wallShift)
			*(m_currentCell + m_width) = VISITED;
		else
			*(m_currentCell + 1) = VISITED;
	else 
		if (m_currentCell - m_currentNeighbour == m_width * m_wallShift)
			*(m_currentCell - m_width) = VISITED;
		else
			*(m_currentCell - 1) = VISITED;
}

int  GLLabyrinth::pm_unvisitedCount() {
	int result = 0;
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			if (m_field[(i * m_width) + j] == EMPTY) result++;
	return result;
}

void GLLabyrinth::pm_setLastCell() {
	static GLuint without_walls = 0;
	m_countWalls = 0;
	for (int i = 0 + without_walls; i < m_height - without_walls; i++)
		for (int j = 0 + without_walls; j < m_width - without_walls; j++)
			if (m_field[(i * m_width) + j] != WALL) m_currentCell = &m_field[(i * m_width) + j];
			else
				m_countWalls++;
}

Vector3f* GLLabyrinth::pm_getVertexCube(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
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

void GLLabyrinth::pm_genWallVBO() {
	m_wallVBO = new GLuint[m_countWalls];
	Vector3f* m_boxColider;
	GLuint begX = m_begPosX, begY = m_begPosY, begZ = m_begPosZ, counter = 0;

	glGenBuffers(m_countWalls, m_wallVBO);// How many, Where store


	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			if (m_field[(i * m_width) + j] == WALL) {
				m_boxColider = pm_getVertexCube(begX, begY, begZ, m_sizeWallX, m_sizeWallY, m_sizeWallZ);
				glBindBuffer(GL_ARRAY_BUFFER, m_wallVBO[counter++]); // Bind type information to buffer
				glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), m_boxColider, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				delete m_boxColider;
			}
			begX += m_sizeWallX;
		}
		begX = m_begPosX;
		begZ += m_sizeWallZ;
	}
}

void GLLabyrinth::pm_genFloorVBO() {
	GLuint sizeX = m_sizeWallX * m_height, sizeZ = m_sizeWallZ * m_width;
	
	glGenBuffers(1, &m_floorVBO);// How many, Where store
	Vector3f* m_boxColider = pm_getVertexCube(m_begPosX, m_begPosY - m_sizeFloorY, m_begPosZ, sizeX, m_sizeFloorY, sizeZ);
	glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), m_boxColider, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete m_boxColider;
}

void GLLabyrinth::pm_genCBO(GLuint * currentCBO, Vector3f currentColor, bool isMotley) {
	glDeleteBuffers(1, &(*currentCBO));
	Vector3f* colorCBO = pm_genColorCBO(currentColor, isMotley);
	glGenBuffers(1, &(*currentCBO));// How many, Where store
	glBindBuffer(GL_ARRAY_BUFFER, *currentCBO); // Bind type information to buffer
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * sizeof(GLfloat), colorCBO, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete colorCBO;
}

Vector3f* GLLabyrinth::pm_genColorCBO(Vector3f currentColor, bool isMotley) {
	Vector3f* out_cube = new Vector3f[8];
	GLuint counter = 0;
	for (int i(0); i < 8; i++)
		if (isMotley)
			out_cube[i] = Vector3f(m_CBO[counter++], m_CBO[counter++], m_CBO[counter++]);
		else
			out_cube[i] = Vector3f(currentColor);
	return out_cube;
}

GLuint GLLabyrinth::pm_getCellType(GLint i, GLint j) {
	if ((i > 0 && j > 0) && (i < m_height && j < m_width))
		return m_field[(i * m_width) + j];
	return WALL;
}

void GLLabyrinth::pm_isThereKey(GLuint i, GLuint j) {
	for (int counterKey(0); counterKey < m_keyArray.GetCount(); counterKey++) 
		if (GetPosI(m_keyArray[counterKey]->GetPosZ()) == i && GetPosJ(m_keyArray[counterKey]->GetPosX()) == j && m_keyArray[counterKey]->GetVisible()){
			m_keyArray[counterKey]->SetVisible(false);
			m_counterGotedKeys++;
			return;
		}
}

bool GLLabyrinth::pm_genChanceKey() {
	GLuint genNumber = rand() % 100;
	return (genNumber >= CHANCE_GEN_KEY) ? false : true;
}

void GLLabyrinth::pm_clearKeys() {
	m_keyArray.RemoveAll();
}

void GLLabyrinth::pm_genKeys() {
	GLBox* tmpBoxKey;
	for (int i(1); i < m_height; i++) {
		for (int j(1); j < m_width; j++) {
			if((i != 1 && j != 1) && (i != m_height - 2 && j != m_width - 2)) 
				if (pm_genChanceKey() && pm_getCellType(i, j) != WALL) {
					tmpBoxKey = new GLBox(1.2, 1.2);
					tmpBoxKey->SetColor(0.2f, 0.7f, 0.2f);
					tmpBoxKey->SetColorWallLines(0.0, 0.0, 0.0);
					tmpBoxKey->CreateBox();
					tmpBoxKey->SetFieldPosition(i, j);
					m_keyArray.Add(tmpBoxKey);
				}
		}
	}
}

bool GLLabyrinth::GetConfirmationMoveWizard(WizardView inView) {
	//std::cout << endl << "I:" << m_posWizardI << " J:" << m_posWizardJ;
	switch (inView)
	{
	case Up:
		if (pm_getCellType(m_posWizardI - 1, m_posWizardJ) != WALL) {
			m_posWizardI--;
			return true;
		}			
		return false;
	case Down:
		if (pm_getCellType(m_posWizardI + 1, m_posWizardJ) != WALL) {
			m_posWizardI++;
			return true;
		}			
		return false;
	case Left:
		if (pm_getCellType(m_posWizardI, m_posWizardJ - 1) != WALL) {
			m_posWizardJ--;
			return true;
		}			
		return false;
	case Right:
		//if(i == 2)
		if (pm_getCellType(m_posWizardI, m_posWizardJ + 1) != WALL) {
			m_posWizardJ++;
			return true;
		}
	default:
		return false;
	}	
	
}

bool GLLabyrinth::GetEndGame() {
	if (m_counterGotedKeys == m_keyArray.GetCount() && (GetPosI(m_exitBox->GetPosZ()) == m_posWizardI && GetPosJ(m_exitBox->GetPosX()) == m_posWizardJ) && m_Wizard->getWizardState() == WizardState::Fixedly)
		return true;
	return false;
}

void GLLabyrinth::GenerateLabyrinth() {
	ReZero();
	m_currentCell = m_field + (1 * m_width) + 1;
	do {
		pm_getNeighbours();
		if (m_neighboursSize > 0) { //если у клетки есть непосещенные соседи
			m_randNum = rand() % m_neighboursSize;
			m_currentNeighbour = m_neighbours[m_randNum]; //выбираем случайного соседа
			m_stackCells.push_back(m_currentCell); //заносим текущую точку в стек
			pm_removeWall(); //убираем стену между текущей и сосендней точками
			m_currentCell = m_currentNeighbour; //делаем соседнюю точку текущей и отмечаем ее посещенной
			*m_currentCell = VISITED;
		}
		else if (m_stackCells.size() > 0) {
			m_currentCell = m_stackCells.back();
			m_stackCells.pop_back();
		}
		else {

		}
	} while (pm_unvisitedCount() > 0);
	pm_setLastCell();
	m_stackCells.clear();
	pm_genWallVBO();
	pm_genFloorVBO();
	// Set IBO
	m_Wizard->SetIBO(m_wallIBO);
	
	m_exitBox = new GLBox(1.2f, 3.0f);
	m_exitBox->SetColor(0.7f, 0.2f, 0.2f);
	m_exitBox->SetColorWallLines(0.0, 0.0, 0.0);
	m_exitBox->CreateBox();
	m_exitBox->SetFieldPosition(m_height - 2, m_width - 2);
	
	pm_genKeys();
}

void GLLabyrinth::GenerateLabyrinth(GLuint width, GLuint height) {
	SetWidth(width);
	SetHeight(height);
	GenerateLabyrinth();
}

void GLLabyrinth::ShowLabyrinthConsole() {
	Clear();
	GotoXY(0, 0);
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++)
			if ((m_currentCell - m_field) / m_height == i && (m_currentCell - m_field) % m_width == j) { // Exit
				SetColor(Red, LightGray);
				cout << "*";
			}
			else {
				if (m_field[(i * m_width) + j] == WALL) {
					SetColor(Black, White);
					cout << " ";
				}
				else {
					SetColor(White, LightGray);
					cout << " ";
				}
			}
			cout << endl;
	}
	for (int i(0); i < m_keyArray.GetCount(); i++) {
		if ( m_keyArray[i]->GetVisible()) {
			GotoXY(GetPosI(m_keyArray[i]->GetPosX()), GetPosJ(m_keyArray[i]->GetPosZ()));
			SetColor(Green, LightGray);
			cout << "*";
		}
	}
	GotoXY(m_posWizardJ, m_posWizardI);
	SetColor(Black, LightGray);
	cout << "*";
}

void GLLabyrinth::ShowLabyrinthGL(/*GLuint vertexID, GLuint fragmentID*/) {// Wizard Position
	glLoadIdentity();
	
	glTranslatef(-m_wizardLocation.x, m_wizardLocation.y + m_wizardLocation.z * 0.87f, -(CAMERA_DISTANSE + m_wizardLocation.z * 0.322f));
	glRotatef(60, 1, 0, 0);

	GLfloat light_position[] = { m_lightPosition.x, m_lightPosition.y , m_lightPosition.z , 1.0 }; // !!!!! Свет + Визард + Враги + Ключ
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Walls
	for (int i = 0; i < m_countWalls; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_wallVBO[i]); // Bind type information to buffer
		glVertexAttribPointer(m_attributeVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_wallCBO); // Bind type information to buffer
		glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallIBO);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindBuffer(GL_ARRAY_BUFFER, m_wallCBOLines); // Bind type information to buffer
		glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallIBOLines);

		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// Floor
	glBindBuffer(GL_ARRAY_BUFFER, m_floorVBO); // Bind type information to buffer
	glVertexAttribPointer(m_attributeVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_floorCBO); // Bind type information to buffer
	glVertexAttribPointer(m_attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_wallIBO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_LIGHTING);
	// Wizard
	//m_Wizard->SetBeginFieldPosition(shiftField.x, shiftField.y, shiftField.z);
	m_Wizard->DrawWizard();
	for (int i(0); i < m_keyArray.GetCount(); i++)
		m_keyArray[i]->DrawBox();
	//m_keyBox->DrawBox();
	if((GetPosI(m_exitBox->GetPosZ()) != m_posWizardI || GetPosJ(m_exitBox->GetPosX()) != m_posWizardJ))
		m_exitBox->DrawBox();
}

void GLLabyrinth::CheckLabyrinthAnimation() { // take coord wizard + shift labyrinth
	m_wizardLocation = m_Wizard->GetFieldPosition();
	if(m_Wizard->getWizardState() == WizardState::Fixedly)
		pm_isThereKey(m_posWizardI, m_posWizardJ);
}