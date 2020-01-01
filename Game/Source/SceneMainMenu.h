#pragma once
#include "Scene.h"

class SceneMainMenu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_TEXT,
		GEO_QUAD,
		GEO_BAR,
		NUM_GEOMETRY,
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

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	Shader defaultShader;

	bool bLightEnabled;
	float fps;

	MS modelStack;
	MS viewStack;
	MS projectionStack;
};