#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "gpch.h"

#include "Camera.h"
#include "Light.h"
#include "GameObject.h"
#include "Player.h"
#include "GOManager.h"
#include "Projectile.h"
#include "Enemy.h"

#include "Scene.h"

#include "Handlers/InputHandler.h"

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

	enum TEXTURE_LIST
	{
		CURSOR_NORMAL,
		CURSOR_CLICKED,

		CARD_SELECTOR,
		CARD_BACK,

		CARD_ELEMENT_PHYS,
		CARD_ELEMENT_FIRE,
		CARD_ELEMENT_COLD,
		CARD_ELEMENT_LTNG,
		CARD_ELEMENT_HEAL,

		CARD_INFO,

		SLOW_BAR,

		LIFE_BAR_FILL,
		LIFE_BAR_LEFT,
		LIFE_BAR_RIGHT,
		LIFE_BAR_LEFT_ANIMAL,
		LIFE_BAR_RIGHT_ANIMAL,

		TEX_COUNT
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

	void FireCard();
	void SelectCard(bool up);
	void ReloadDeck();
private:
	unsigned m_vertexArrayID;

	// UI textures
	GLuint textures[TEX_COUNT];

	Mesh* meshList[NUM_GEOMETRY];
	Shader defaultShader;
	GOManager* GOMan;
	Camera camera;

	SpriteAnimation* blankSprite;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;
	float fps;

	Sprite BaseProjectile;
	Sprite BaseSpell;

	GameObject* cursorGO;

	vec3 clickpos;
	Player* player;
	std::vector<Projectile*> projectiles;
	std::vector<Spell*> spells;
	std::vector<Enemy*> enemies;

	InputHandler ih;
	Timer whenToSpawn;

	float slowMoTimer;

	bool skipPollMouse = false;

	unsigned short selectedCard;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
};

#endif