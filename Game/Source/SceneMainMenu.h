#pragma once
#include "Scene.h"

struct MenuItem
{
	MenuItem()
	{
		x = y = w = h = AnchorPoint = 0;
	}

	MenuItem(float x, float y, float w, float h, unsigned char anchor)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		AnchorPoint = anchor;
	}

	int x, y, w, h;
	unsigned char AnchorPoint;
};

class SceneMainMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_TEXT,
		GEO_QUAD,
		GEO_BAR,
		NUM_GEOMETRY,
	};	
	
	enum TEXTURE_LIST
	{
		LOGO,
		BUTTON,
		CURSOR,

		TEX_COUNT
	};

public:
	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderMesh(Mesh *mesh, bool enableLight);

	int Button(MenuItem m, std::string text);

	std::string hotitem, activeitem;

private:
	GLuint textures[TEX_COUNT];
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	Shader defaultShader;

	bool bLightEnabled;
	float fps;

	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

};