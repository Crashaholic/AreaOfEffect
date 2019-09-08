#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include "shader.hpp"
#include <vector>
#include "Player.h"
#include "GOManager.h"
#include "Projectile.h"
#include "Enemy.h"

class SceneGame : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_TEXT,
		GEO_BALL,
		GEO_QUAD,
		GEO_BAR,
		NUM_GEOMETRY,
	};
public:
	SceneGame();
	~SceneGame();

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
	GOManager GOMan;
	Camera camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;
	float fps;

	GameObject cursorGO;

	//Player p;
	Player* player;
	vec3 clickpos;
	Projectile projectile;
	std::vector<Projectile*> projectileList;

	//Physics
	std::vector<Entity*> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
};

#endif