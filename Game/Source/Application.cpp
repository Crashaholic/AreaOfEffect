#include "gpch.h"
#include "Application.h"

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneGame.h"
#include "SceneMainMenu.h"
#include "Handlers/SceneHandler.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
int m_width, m_height;


//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


void resize_callback(GLFWwindow* window, int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, w, h);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
	short* temp = &(Application::GetInstance().scrollState);
	if (y > 0)
		*temp = 1;
	else if (y < 0)
		*temp = -1;
}

bool Application::IsKeyPressed(unsigned short key)
{
	if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED))
		return ((GetAsyncKeyState(key) & 0x8001) != 0);
	return NULL;
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED))
		return glfwGetMouseButton(m_window, key) != 0;
	return NULL;
}
void Application::GetCursorPos(double *xpos, double *ypos)
{
	if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED))
		glfwGetCursorPos(m_window, xpos, ypos);
	else
		xpos = 0; ypos = 0;
}
int Application::GetWindowWidth()
{
	return m_width;
}
int Application::GetWindowHeight()
{
	return m_height;
}

GLFWwindow * Application::GetWindow()
{
	return m_window;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
	glfwWindowHint(GLFW_RESIZABLE, false);

	usrsttngs.LoadSettings("game.ini");

	//Create a window and create its OpenGL context
	m_width = usrsttngs.windowWidth;
	m_height = usrsttngs.windowHeight;
	if (!usrsttngs.windowedMode)
	{
		glfwWindowHint(GLFW_DECORATED, false);
	}
	m_window = glfwCreateWindow(m_width, m_height, "AreaOfEffect", usrsttngs.windowFullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glfwSetScrollCallback(m_window, scroll_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

#ifdef _DEBUG
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#else
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif

	SceneHandler::GetInstance()->AddScene<SceneGame>();
	SceneHandler::GetInstance()->AddScene<SceneMainMenu>();
}

void Application::Run()
{
	//Main Loop
	//std::unique_ptr<Scene> scene = std::make_unique<SceneGame>();
	//scene->Init();

	try
	{
		//SceneHandler::GetInstance()->StartUpScene<SceneGame>();
		SceneHandler::GetInstance()->StartUpScene<SceneMainMenu>();
	}
	catch (SceneNotRegistered e)
	{
		std::cout << e.what() << '\n';
		__debugbreak();
	}

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		//scene->Update(m_timer.getElapsedTime());
		//scene->Render();
		SceneHandler::GetInstance()->RunScene(m_timer.getElapsedTime());
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		scrollState = 0;
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   


	} //Check if the ESC key had been pressed or if the window had been closed
	//scene->Exit();
	SceneHandler::GetInstance()->CloseScene();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
