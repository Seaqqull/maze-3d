#include "GLWindow.h"

GLfloat GLBox::m_sizeWallX = 0;
GLfloat GLBox::m_sizeWallZ = 0;
GLuint GLBox::m_IBO = 0;
GLuint GLBox::m_IBOLines = 0;
GLuint GLBox::m_attributeColor = 0;
GLuint GLBox::m_attributeVertex = 0;

GLWindow::GLWindow()
{
	m_windowTitle = "Test";

	m_windowHeight = 900;
	m_windowWidth = 1600;

	m_screenMode = false;

	m_fontStyle = GLUT_BITMAP_HELVETICA_18;

	pm_CheckCorrectnessSize(m_windowWidth, m_windowHeight);
	pm_InitWindow();
}

GLWindow::GLWindow(String windowTitle, int windowWidth, int windowHeight, bool fullScreen = false) {
	m_windowTitle = windowTitle;
	
	m_windowHeight = windowHeight;
	m_windowWidth = windowWidth;

	m_screenMode = fullScreen;

	m_fontStyle = GLUT_BITMAP_TIMES_ROMAN_24;
	
	pm_CheckCorrectnessSize(m_windowWidth, m_windowHeight);
	pm_InitWindow();

	// Shaders
	InitShaders();
	//glm::mat4
	shaderProgram->AttachShader(&vertexShader->GetShaderID());
	shaderProgram->AttachShader(&fragmentShader->GetShaderID());
	shaderProgram->LinkProgram();

	shaderProgram->AttachAttribute("in_vertex");
	shaderProgram->AttachAttribute("in_color");
	
	m_labyrinth = new GLLabyrinth(21, 21);

	GLBox::SetLabyrinthSizes(m_labyrinth->GetSizeX(), m_labyrinth->GetSizeZ());
	GLBox::SetAttribyte(shaderProgram->GetAttribute("in_vertex"), shaderProgram->GetAttribute("in_color"));

	m_wizard = new GLWizard(2.0f, 5.0f);

	
	m_wizard->SetAttribyte(shaderProgram->GetAttribute("in_vertex"), shaderProgram->GetAttribute("in_color"));
	//m_wizard->SetIBO(m_wallIBO); IBO in GenerateLabyrinth
	m_wizard->setColorBody(0.9, 0, 0);
	m_wizard->setColorHead(0.23f, 0.89f, 0.81f, 0.70f, 0.31f, 0.91f, false);
	m_wizard->setColorGlass(0.39f, 0.58f, 0.93f);
	m_wizard->setLabyrinthSizes(m_labyrinth->GetSizeX(), m_labyrinth->GetSizeZ());
	m_wizard->CreateWizard();

	m_labyrinth->m_Wizard = m_wizard;

	m_labyrinth->SetAttribyte(shaderProgram->GetAttribute("in_vertex"), shaderProgram->GetAttribute("in_color"));
	m_labyrinth->SetColorWall(0.97f, 0.97f, 1.0f, false);
	m_labyrinth->SetColorWallLines(0.0f, 0.0f, 0.0f, false);
	m_labyrinth->SetColorFloor(0.5f, 0.5f, 0.5f, false);

	pm_newGame();
	m_gamePauseMode = true;
	pm_readData();
}

void GLWindow::pm_newGame() {
	m_ShowFPS = true, m_gamePauseMode = false, m_gameModeStarted = false, m_recordsMode = false;// recordsMode
	m_frameCount = 0;
	m_FPS = 0;
	m_timeCurrent = 0, m_timePrevious = 0;
	m_rotateX = 0, m_rotateY = 0, m_rotateZ = 0;
	m_menuItem = 0;
	m_recordsFirstRec = 0;
	m_colorInterface = Vector3f(1.0f, 0.55f, 0.1f);
	m_timeBeginGame = CTime::GetCurrentTime();
	m_timeBeginPause = m_timeBeginGame;
	m_labyrinth->GenerateLabyrinth();
	m_wizard->ReZero();
	m_labyrinth->ShowLabyrinthConsole();
}


void GLWindow::pm_InitWindow() {
	//  create a window with the specified dimensions
	glutInitWindowSize(m_windowWidth, m_windowHeight);


	//  Set the window x and y coordinates such that the 
	//  window becomes centered
	pm_CenterOnScreen();

	//  Position Window
	glutInitWindowPosition(m_windowX, m_windowY);

	//  Set Display mode
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	//  Create window with the specified title
	m_windowHandle = glutCreateWindow(m_windowTitle);
	
	//  View in full screen if the m_screenMode flag is on
	if (m_screenMode)
		glutFullScreen();

	// Set Game Cursor
	//glutSetCursor(GLUT_CURSOR_NONE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Init GLEW
	GLenum GlewInitResult;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult)
		exit(EXIT_FAILURE);

	SetBkColor(0, 0, 0);
}

//-------------------------------------------------------------------------
//  This function sets the window x and y coordinates
//  such that the window becomes centered
//-------------------------------------------------------------------------
void GLWindow::pm_CenterOnScreen()
{
	m_windowX = (glutGet(GLUT_SCREEN_WIDTH) - m_windowWidth) / 2;
	m_windowY = (glutGet(GLUT_SCREEN_HEIGHT) - m_windowHeight) / 2;
}
void GLWindow::pm_CheckCorrectnessSize(int& w, int& h) {
	GLint width = glutGet(GLUT_SCREEN_WIDTH), height = glutGet(GLUT_SCREEN_HEIGHT);
	if (w < width && h < height)
		return;
	if (width == 1920 && height == 1080) {
		w = 1600;
		h = 900;
	}
	else if (width == 1600 && height == 900) {
		w = 1366;
		h = 768;
	}
	else if (width <= 1366 && height <= 768 || (width <= 800 || height <= 600)) {
		w = 800;
		h = 600;
	}
}

//  Set the frame buffer clear color 
void GLWindow::SetBkColor(GLuint red, GLuint green, GLuint blue) {
	glClearColor((1.0f / 255) * red, (1.0f / 255) * green, (1.0f / 255) * blue, 1);
}

void GLWindow::OnResizeWindow(int w, int h) {
	m_windowWidth = w;
	m_windowHeight = h;

	//  Reset viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*if (m_windowWidth <= m_windowHeight)
		glOrtho(-1.0, 1.0,
			-1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth, 1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth, -200, 200);
	else
		glOrtho(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight,
			1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, -1.0, 1.0, -200, 200);*/
	gluPerspective(60, 1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 0.1, 200);
	/*if (m_windowWidth <= m_windowHeight)
		glOrtho(-1.0, 1.0,
			-1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth, 1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth, -200, 200);
	else
		glOrtho(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight,
			1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, -1.0, 1.0, -200, 200);*/

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWindow::OnKeyboardCB(unsigned char Key, int x, int y) {
	switch (Key) {
	case 'q':
		exit(0);
	case '/':
		m_screenMode = !m_screenMode;
		if (m_screenMode)
			glutFullScreen();
		else {
			glutReshapeWindow(1600, 900);
			OnResizeWindow(1600, 900);
			pm_CenterOnScreen();
			glutPositionWindow(m_windowX, m_windowY);
		}
		return;
	case 'w':
		if (m_gamePauseMode) {
			if (m_recordsMode) {
				if (m_recordsFirstRec > 0)	m_recordsFirstRec--;
			}
			else
				if (m_menuItem > 0)
					m_menuItem--;
		}
		else
			if(m_wizard->getWizardState() == WizardState::Fixedly)
				if (m_wizard->getWizardView() == WizardView::Up && m_labyrinth->GetConfirmationMoveWizard(WizardView::Up)) {
					m_labyrinth->ShowLabyrinthConsole();
					m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Up, WizardState::NoneState);
				}				
				else if(m_wizard->getWizardView() != WizardView::Up)
					m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Up, WizardState::NoneState);
		return;
	case 's':
		if (m_gamePauseMode) {
			if (m_recordsMode) {
				if (m_recordsFirstRec + RECORDS_PER_PAGE < (m_records.GetCount() / 3) )	m_recordsFirstRec++;
			}
			else
				if ((m_menuItem < 3 && m_gameModeStarted) || (m_menuItem < 2 && !m_gameModeStarted))
					m_menuItem++;
		}
		else
			if (m_wizard->getWizardState() == WizardState::Fixedly)
				if (m_wizard->getWizardView() == WizardView::Down && m_labyrinth->GetConfirmationMoveWizard(WizardView::Down)) {
					m_labyrinth->ShowLabyrinthConsole();
					m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Down, WizardState::NoneState);
				}				
				else if (m_wizard->getWizardView() != WizardView::Down)
					m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Down, WizardState::NoneState);
		return;
	case 'a':
		if (m_wizard->getWizardState() == WizardState::Fixedly)
			if (m_wizard->getWizardView() == WizardView::Left && m_labyrinth->GetConfirmationMoveWizard(WizardView::Left)) {
				m_labyrinth->ShowLabyrinthConsole();
				m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Left, WizardState::NoneState);
			}				
			else if (m_wizard->getWizardView() != WizardView::Left)
				m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Left, WizardState::NoneState);	
		return;
	case 'd':
		if (m_wizard->getWizardState() == WizardState::Fixedly)
			if (m_wizard->getWizardView() == WizardView::Right && m_labyrinth->GetConfirmationMoveWizard(WizardView::Right)) {
				m_labyrinth->ShowLabyrinthConsole();
				m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Right, WizardState::NoneState);
			}				
			else if (m_wizard->getWizardView() != WizardView::Right)
				m_wizard->SetWizardState(glutGet(GLUT_ELAPSED_TIME), WizardView::Right, WizardState::NoneState);
		return;
	case 'n':
		pm_newGame();
		break;
	case '\x1b':// Esc
		if (!m_gamePauseMode)
			pm_pauseGame();
		else if (m_recordsMode)
			m_recordsMode = false;
		else if (m_gamePauseMode && !m_gameModeStarted)
			exit(0);
		else if (m_gamePauseMode && m_gameModeStarted)
			pm_resumeGame();
		break;
	case '\r':// Enter
		if (m_gamePauseMode) {
			if (m_menuItem == 0 && m_gameModeStarted) { // Resume
				pm_resumeGame();
			}
			else if ((m_menuItem == 0 && !m_gameModeStarted) || (m_menuItem == 1 && m_gameModeStarted)) // New Game
			{
				pm_newGame();
				pm_resumeGame();
			}
			else if ((m_menuItem == 1 && !m_gameModeStarted) || (m_menuItem == 2 && m_gameModeStarted)) {
				m_recordsMode = true;
				m_recordsFirstRec = 0;
			}
			else if ((m_menuItem == 2 && !m_gameModeStarted) || (m_menuItem == 3 && m_gameModeStarted)) { // Exit
				exit(0);
			}
		}
		break;
	}

}
void GLWindow::pm_pauseGame() {
	m_menuItem = 0;
	m_timeBeginPause = CTime::GetCurrentTime();
	m_gamePauseMode = true;
}

void GLWindow::pm_resumeGame() {
	//GLuint curTime = glutGet(GLUT_ELAPSED_TIME);
	CTimeSpan tmpSpan = CTime::GetCurrentTime() - m_timeBeginPause;
	m_timeBeginGame += tmpSpan;
	m_gameModeStarted = true;
	m_gamePauseMode = false;
}

void GLWindow::pm_writeData() {
	std::ofstream Ofs;
	Ofs.open("Records.txt", std::ios::out);
	String* buffStr;

	buffStr = new String(SetWordw("%02ix%02i", m_labyrinth->GetHeight(), m_labyrinth->GetWidth()));
	Ofs << (char*)(*buffStr) << '\n';
	buffStr = new String(SetWordw("%02i", m_labyrinth->GetCurrentKeys()));
	Ofs << (char*)(*buffStr) << '\n';
	buffStr = new String(SetWordw("%02i:%02i", currentTime.GetMinutes(), currentTime.GetSeconds()));
	Ofs << (char*)(*buffStr) << '\n';

	for (int i(0); i < m_records.GetCount() && i < 57; i++) {// 19 записей * 3 строки
		Ofs << (char*)(*m_records[i]);
		if (i + 1 < m_records.GetCount() && i + 1 < 57)
			Ofs << '\n';
	}
	Ofs.close();
}

void GLWindow::pm_readData() {
	ifstream Ifs("Records.txt");
	char buff[6];
	GLuint counter = 0;
	m_records.RemoveAll();
	if (Ifs.is_open())
		while (!Ifs.eof() && counter < 57) // 19 записей * 3 строки
		{
			Ifs >> buff;
			m_records.Add(new String(buff));
			counter++;
		}
	Ifs.close();
	if (m_records.GetCount() % 3 != 0)
		m_records.RemoveAll();
}

void GLWindow::ShowMenu() {
	glPushMatrix();
	glLoadIdentity();
	glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);

	if (m_gameModeStarted) {
		if(m_menuItem == 0)
			glColor3f(0.2f, 0.7f, 0.2f);
		else
			glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
		Printw(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 0.95f, -1.8, "%s", "Resume");
	}
	if ((m_menuItem == 0 && !m_gameModeStarted) || (m_menuItem == 1 && m_gameModeStarted))
		glColor3f(0.2f, 0.7f, 0.2f);
	else
		glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
	Printw(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 0.85f, -1.8, "%s", "New Game");
	
	if ((m_menuItem == 1 && !m_gameModeStarted) || (m_menuItem == 2 && m_gameModeStarted))
		glColor3f(0.2f, 0.7f, 0.2f);
	else
		glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
	Printw(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 0.75f, -1.8, "%s", "Records");

	if ((m_menuItem == 2 && !m_gameModeStarted) || (m_menuItem == 3 && m_gameModeStarted))
		glColor3f(0.2f, 0.7f, 0.2f);
	else
		glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
	Printw(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, 0.65f, -1.8, "%s", "Exit");

	glPopMatrix();
}
void GLWindow::ShowRecords() {
	glPushMatrix();
	glLoadIdentity();
	glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
	GLuint counter = 0;
	Printw(-0.3, 0.55f - 0.1 * counter, -1.8, "%s", "Size");
	Printw(-0.1, 0.55f - 0.1 * counter, -1.8, "%s", "Keys");
	Printw(0.1, 0.55f - 0.1 * counter, -1.8, "%s", "Time");
	for (int i(0 + m_recordsFirstRec); i < m_records.GetCount() / 3 && counter < RECORDS_PER_PAGE; i++) {
		Printw(-0.32, 0.55f - 0.1 * (counter + 1), -1.8, "%s", (char*)(*m_records[i * 3]));
		Printw(-0.07, 0.55f - 0.1 * (counter + 1), -1.8, "%s", (char*)(*m_records[i * 3 + 1]));
		Printw(0.1, 0.55f - 0.1 * (counter + 1), -1.8, "%s", (char*)(*m_records[i * 3 + 2]));
		counter++;
	}
}

void GLWindow::OnDisplay(void) {
	EnableGLSmooth();
	DisableGLLightning();
	if (m_gamePauseMode) {
		ShowMenu();
		if (m_recordsMode)
			ShowRecords();
	}
	else {
		
		// Draw
		//
		EnableGLLightning();
		shaderProgram->ConnectProgram();

		shaderProgram->EnableAttribute("in_vertex");
		shaderProgram->EnableAttribute("in_color");


		m_labyrinth->ShowLabyrinthGL(/*shaderProgram->GetAttribute("in_vertex"), shaderProgram->GetAttribute("in_color")*/);
		/*glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);*/


		shaderProgram->DisableAttribute("in_vertex");
		shaderProgram->DisableAttribute("in_color");

		///...
		// Compute the MVP matrix from keyboard and mouse input

		/*glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;*/

		//glEnableVertexAttribArray(0);
		//glBindBuffer(GL_ARRAY_BUFFER, vertV); // Bind type information to buffer
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, vertC); // Bind type information to buffer
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(tria_vert) / (sizeof(GLfloat) * 3));// 9

		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		///...

		/*glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);*/

		shaderProgram->DisconnectProgram();
		//
		DisableGLLightning();

		
		// Draw FPS
		if (m_ShowFPS)
			DrawFPS();
		DrawKeys();
		DrawTime();
	}
	DisableGLSmooth();
	glutSwapBuffers();
}

void GLWindow::OnIdle(void)
{
	m_timeCurrent = glutGet(GLUT_ELAPSED_TIME);
	static GLuint previousAnTime = 0;
	GLuint timeInterval = m_timeCurrent - previousAnTime;

	if (timeInterval > 1000 / 60 && !m_gamePauseMode) {
		// Animate 
		m_wizard->CheckWizardAnimation(m_timeCurrent);
		m_labyrinth->CheckLabyrinthAnimation();
		// Time
		previousAnTime = m_timeCurrent;
	}
	// Calculate FPS
	CalculateFPS();
	//  Call display function (draw the current frame)
	glutPostRedisplay();
	if (m_labyrinth->GetEndGame()) {
		pm_writeData();
		pm_readData();
		pm_newGame();
		pm_pauseGame();
		m_recordsMode = true;
		m_recordsFirstRec = 0;
	}
}

void GLWindow::SetMainWindow() {
	//  Start GLUT event processing loop
	glutMainLoop();
}

void GLWindow::CalculateFPS() {
	//  Increase frame count
	m_frameCount++;

	//  Get the number of milliseconds since glutInit called 
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	m_timeCurrent = glutGet(GLUT_ELAPSED_TIME);

	//  Calculate time passed
	int timeInterval = m_timeCurrent - m_timePrevious;

	if (timeInterval > 1000)
	{
		//  calculate the number of frames per second
		m_FPS = m_frameCount / (timeInterval / 1000.0f);

		//  Set time
		m_timePrevious = m_timeCurrent;

		//  Reset frame count
		m_frameCount = 0;

		/*GLLabyrinth* lab = new GLLabyrinth(15, 15);
		lab->GenerateLabyrinth();
		lab->ShowLabyrinth();*/
	}
}
void GLWindow::DrawFPS() {
	glPushMatrix();

	glLoadIdentity();
	glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
	//  Print the FPS to the window
	if (m_windowWidth <= m_windowHeight)
		Printw(1.0 - 0.3, -1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth + 0.05f, -1.8, "FPS: %4.2f", m_FPS);
	else
		Printw(1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight - 0.25, -0.95f, -1.8, "FPS: %4.2f", m_FPS);

	glPopMatrix();
}

void GLWindow::DrawKeys() {
	glPushMatrix();
	
	glLoadIdentity();
	glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);
	
	if (m_windowWidth <= m_windowHeight)
		Printw( 0.1, -1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth + 0.05f, -1.8, "Keys: %2.0d / %d", m_labyrinth->GetCurrentKeys(), m_labyrinth->GetOverallKeys());
	else
		Printw(-1.0*(GLfloat)m_windowWidth / (GLfloat)m_windowHeight, -0.95f, -1.8, "Keys: %2.0d / %d", m_labyrinth->GetCurrentKeys(), m_labyrinth->GetOverallKeys());

	glPopMatrix();
}

void GLWindow::DrawTime() {
	glPushMatrix();
	currentTime = CTime::GetCurrentTime() - m_timeBeginGame;

	glLoadIdentity();
	glColor3f(m_colorInterface.x, m_colorInterface.y, m_colorInterface.z);

	if (m_windowWidth <= m_windowHeight)
		Printw(0.1, 1.0*(GLfloat)m_windowHeight / (GLfloat)m_windowWidth + 0.05f, -1.8, "Time: %02i:%02i", currentTime.GetMinutes(), currentTime.GetSeconds());
	else
		Printw(0.0f, 0.95f, -1.8, "Time: %02i:%02i", currentTime.GetMinutes(), currentTime.GetSeconds());

	glPopMatrix();
}

void GLWindow::Printw(float x, float y, float z, char* format, ...) {
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text

					//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list 
	va_end(args);

	//  Specify the raster position for pixel operations.
	glRasterPos3f(x, y, z);

	//  Draw the characters one by one
	for (i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(m_fontStyle, text[i]);

	//  Free the allocated memory for the string
	free(text);
}

char* GLWindow::SetWordw(char* format, ...) {
	va_list args;	//  Variable argument list
	int len;		//	String length
	int i;			//  Iterator
	char * text;	//	Text
					//  Initialize a variable argument list
	va_start(args, format);

	//  Return the number of characters in the string referenced the list of arguments.
	//  _vscprintf doesn't count terminating '\0' (that's why +1)
	len = _vscprintf(format, args) + 1;

	//  Allocate memory for a string of the specified size
	text = (char *)malloc(len * sizeof(char));

	//  Write formatted output using a pointer to the list of arguments
	vsprintf_s(text, len, format, args);

	//  End using variable argument list 
	va_end(args);

	return text;
}

void GLWindow::InitShaders() {
	shaderProgram = new GLShaderProgram();
	vertexShader = new GLShader(GL_VERTEX_SHADER, &shaderProgram->GetProgram(), VertexShader);
	fragmentShader = new GLShader(GL_FRAGMENT_SHADER, &shaderProgram->GetProgram(), FragmentShader);
}

//-------------------------------------------------------------------------
//  OpenGL enable different smooth
//-------------------------------------------------------------------------
void GLWindow::EnableGLSmooth() {
	glLineWidth(4.0f);
	glShadeModel(GL_SMOOTH);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);// буфер цвета и глубину
	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//// Stencil
	//glEnable(GL_STENCIL_TEST);
	//glEnable(GL_EXT_stencil_two_side);
	// Blend
	/*glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);*/
	// Сглаживание точек
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	// Сглаживание линий
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	// Сглаживание полигонов    
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	EnableGLLightning();
}

//-------------------------------------------------------------------------
//  OpenGL disable different smooth
//-------------------------------------------------------------------------
void GLWindow::DisableGLSmooth() {
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
	/*glDisable(GL_BLEND);*/
	/*glDisable(GL_STENCIL_TEST);*/
	glDisable(GL_DEPTH_TEST);

	DisableGLLightning();
}

void GLWindow::EnableGLLightning() {
	// lightning
	glEnable(GL_LIGHTING); // Подключение света
	GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);

	//GLfloat light_position[] = { 0.5f, 0.5f, 0.65f, 0.0 };
	//GLfloat light_diffuse[] = { 0.98f, 1.0f, 0.48f };

	float am[4] = { 1,1,1,1 };
	float dif[4] = { 0.98f, 1.0f, 1.0f };
	float sp[4] = { 1,1,1,1 };


	float whiteLight[4] = { 0,0,0,1 }; // сложение с AMBIENT 
									   //(по умолчанию 0.2 0.2 0.2)
									   // если это не установить то будет больше на 0.2 

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // для обоих сторон
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, am);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sp);
	// двухсторонний расчет освещения
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // для обоих сторон
	// автоматическое приведение нормалей к
	// единичной длине
	//glEnable(GL_NORMALIZE);
}

void GLWindow::DisableGLLightning() {
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);	
}

GLWindow::~GLWindow()
{
}
