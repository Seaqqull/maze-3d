//#define MY_DEBUG_MODE
#include "GLWindow.h"

#include <iostream>

GLWindow* mainWnd;



void InitializeGlutCallbacks();

void callback_Idle(void);
void callback_Display(void);
void callback_Reshape(int w, int h);
void callback_KeyboardCB(unsigned char Key, int x, int y);

void main(int argc, char **argv) {
	// Connect to the windowing system
	glutInit(&argc, argv);

	mainWnd = new GLWindow("Maze", 1600, 900, false);

	mainWnd->SetBkColor(69, 77, 87);

	InitializeGlutCallbacks();

	mainWnd->SetMainWindow();

	exit(EXIT_SUCCESS);
}


//-------------------------------------------------------------------------
//  Set OpenGL callbak functions
//-------------------------------------------------------------------------
void InitializeGlutCallbacks()
{
	glutIdleFunc(callback_Idle);
	glutDisplayFunc(callback_Display);
	glutReshapeFunc(callback_Reshape);
	glutKeyboardFunc(callback_KeyboardCB);
	/*
	glutSpecialFunc(SpecialKeyboardCB);
	glutPassiveMotionFunc(PassiveMouseCB);
	*/
}

//-------------------------------------------------------------------------
//  This function is called when OpenGL\GLUT is not working on 
//  something else... It is mainly used for animation... 
//
//  It's like the timers but time intervals are dependent on how busy
//  the app is, instead of having a constant value set by the user.
//-------------------------------------------------------------------------
void callback_Idle(void) {
	mainWnd->OnIdle();
}

//-------------------------------------------------------------------------
//  This function is passed to glutDisplayFunc in order to display 
//	OpenGL contents on the window.
//-------------------------------------------------------------------------
void callback_Display(void) {
	mainWnd->OnDisplay();
}

//-------------------------------------------------------------------------
//  This function is passed to the glutReshapeFunc and is called 
//  whenever the window is resized.
//-------------------------------------------------------------------------
void callback_Reshape(int w, int h) {
	glutReshapeWindow(mainWnd->GetWindowWidth(), mainWnd->GetWindowHeight());
	mainWnd->OnResizeWindow(mainWnd->GetWindowWidth(), mainWnd->GetWindowHeight());
}

//-------------------------------------------------------------------------
//  This function is passed to the glutKayFunc
//  whenever the key is pressed.
//-------------------------------------------------------------------------
void callback_KeyboardCB(unsigned char Key, int x, int y) {
	mainWnd->OnKeyboardCB(Key, x, y);
}
