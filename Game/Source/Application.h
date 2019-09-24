
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Handlers/SettingsHandler.h"

typedef struct GLFWwindow GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();
	
	GLFWwindow* GetWindow();

	GameUserSettings usrsttngs;

	short scrollState;

	bool windowHasFocus;

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
};

#endif