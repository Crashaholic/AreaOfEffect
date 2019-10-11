#include "gpch.h"
#include "SceneGame.h"
#include "Application.h"
#include <iomanip>

#define CircleCollision(a, b) (b->GO->pos.x - a->GO->pos.x) * (b->GO->pos.x - a->GO->pos.x) + \
                              (b->GO->pos.y - a->GO->pos.y) * (b->GO->pos.y - a->GO->pos.y) <= \
                              (b->GO->scale / 2.0f + a->GO->scale / 2.0f) * (b->GO->scale / 2.0f + a->GO->scale / 2.0f)

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	Math::InitRNG();

	m_worldHeight = 20.f;
	m_worldWidth = 20.f;
	
	GOMan = new GOManager;

	defaultShader.Init("Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	defaultShader.Use();

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);
	
	defaultShader.SetInt("numLights", (int)0);
	defaultShader.SetBool("textEnabled", (bool)0);

	defaultShader.SetInt  ("lights[0].type"     ,lights[0].type);
	defaultShader.SetVec3 ("lights[0].color"    ,vec3{ lights[0].color.r, lights[0].color.g ,lights[0].color.b });
	defaultShader.SetFloat("lights[0].power"    ,lights[0].power);
	defaultShader.SetFloat("lights[0].kC"       ,lights[0].kC);
	defaultShader.SetFloat("lights[0].kL"       ,lights[0].kL);
	defaultShader.SetFloat("lights[0].kQ"       ,lights[0].kQ);
	defaultShader.SetFloat("lights[0].cosCutoff",lights[0].cosCutoff);
	defaultShader.SetFloat("lights[0].cosInner" ,lights[0].cosInner);
	defaultShader.SetFloat("lights[0].exponent" ,lights[0].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1);
	meshList[GEO_BAR]  = MeshBuilder::GenerateBar("bar", Color(1, 1, 1), 1);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//couriernew.tga", false);
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	meshList[GEO_QUAD]->transparency = 0.5f;
	meshList[GEO_BAR]->transparency = 0.5f;

	blankSprite = MeshBuilder::GenerateSpriteAnimation("a", 1, 1);

	textures[TEXTURE_LIST::CURSOR_NORMAL        ] = Load::TGA("Image//cursor.tga");
	textures[TEXTURE_LIST::CURSOR_CLICKED       ] = Load::TGA("Image//cursor_clicked.tga");
	textures[TEXTURE_LIST::CARD_SELECTOR        ] = Load::TGA("Image//selector.tga");
	textures[TEXTURE_LIST::CARD_BACK            ] = Load::TGA("Image//card_drawn.tga");
	textures[TEXTURE_LIST::CARD_ELEMENT_PHYS    ] = Load::TGA("Image//phys_ico.tga");
	textures[TEXTURE_LIST::CARD_ELEMENT_FIRE    ] = Load::TGA("Image//fire_ico.tga");
	textures[TEXTURE_LIST::CARD_ELEMENT_COLD    ] = Load::TGA("Image//cold_ico.tga");
	textures[TEXTURE_LIST::CARD_ELEMENT_LTNG    ] = Load::TGA("Image//ltng_ico.tga");
	textures[TEXTURE_LIST::CARD_ELEMENT_HEAL    ] = Load::TGA("Image//heal_ico.tga");
	textures[TEXTURE_LIST::CARD_INFO            ] = Load::TGA("Image//blk.tga");
	textures[TEXTURE_LIST::SLOW_BAR             ] = Load::TGA("Image//gradbar.tga");
	textures[TEXTURE_LIST::LIFE_BAR_FILL        ] = Load::TGA("Image//life_mid.tga");
	textures[TEXTURE_LIST::LIFE_BAR_LEFT        ] = Load::TGA("Image//life_left.tga");
	textures[TEXTURE_LIST::LIFE_BAR_RIGHT       ] = Load::TGA("Image//life_right.tga");
	textures[TEXTURE_LIST::LIFE_BAR_LEFT_ANIMAL ] = Load::TGA("Image//life_left_lion.tga");
	textures[TEXTURE_LIST::LIFE_BAR_RIGHT_ANIMAL] = Load::TGA("Image//life_right_wolf.tga");

	cursorGO = new GameObject();

	cursorGO->sprites["NORMAL"].first = MeshBuilder::GenerateSpriteAnimation("a", 1, 1);
	cursorGO->sprites["NORMAL"].second = textures[TEXTURE_LIST::CURSOR_NORMAL];

	cursorGO->sprites["CLICK"].first = MeshBuilder::GenerateSpriteAnimation("a", 1, 1);
	cursorGO->sprites["CLICK"].second = textures[TEXTURE_LIST::CURSOR_CLICKED];

	cursorGO->activeSprite = cursorGO->sprites["NORMAL"];

	bLightEnabled = false;

	m_speed = 1.f;
	slowMoTimer = 3.f;
	selectedCard = 2;

	BaseProjectile.first = MeshBuilder::GenerateSpriteAnimation("proj", 1, 4);
	BaseProjectile.first->m_anim = new Animation();
	BaseProjectile.first->m_anim->Set(0, 3, 0.25f, true, true);
	BaseProjectile.second = Load::TGA("Image//projectile.tga");

	BaseSpell.first = MeshBuilder::GenerateSpriteAnimation("spel", 1, 1);
	BaseSpell.second = Load::TGA("Image//range.tga");
	BaseSpell.first->transparency = 0.5f;

	player = new Player();
	player->Init(GOMan->FetchGO());
	player->InitCam(&camera);
	player->GO->pos.z = 6;
	player->GO->mass = 10;
	player->GO->scale = 3;
	
	player->GO->sprites["IDLE"].first = MeshBuilder::GenerateSpriteAnimation("player_idle", 1, 4);
	player->GO->sprites["IDLE"].first->m_anim = new Animation();
	player->GO->sprites["IDLE"].first->m_anim->Set(0, 3, 2.f, true, true);
	player->GO->sprites["IDLE"].second = Load::TGA("Image//player_idle.tga");
	
	player->GO->sprites["MOVE_X_LEFT"].first = MeshBuilder::GenerateSpriteAnimation("player_move_x_left", 1, 4);
	player->GO->sprites["MOVE_X_LEFT"].first->m_anim = new Animation();
	player->GO->sprites["MOVE_X_LEFT"].first->m_anim->Set(0, 3, 0.5f, true, true);
	player->GO->sprites["MOVE_X_LEFT"].second = Load::TGA("Image//player_move_x_left.tga");
	
	player->GO->sprites["MOVE_X_RIGHT"].first = MeshBuilder::GenerateSpriteAnimation("player_move_x_right", 1, 4);
	player->GO->sprites["MOVE_X_RIGHT"].first->m_anim = new Animation();
	player->GO->sprites["MOVE_X_RIGHT"].first->m_anim->Set(0, 3, 0.5f, true, true);
	player->GO->sprites["MOVE_X_RIGHT"].second = Load::TGA("Image//player_move_x_right.tga");
	
	player->GO->activeSprite = player->GO->sprites["IDLE"];

	player->maxHealth = 10.f;
	player->health = 10.f;

	Spell temp1;
	temp1.delay = 0.0f;
	temp1.radius = 1;
	temp1.duration = 5;
	temp1.dmg = Damage{0, 1, 0, 0};

	player->resistance.Phys = 75.f;
	player->resistance.Fire = 75.f;
	player->resistance.Cold = 75.f;
	player->resistance.Light = 75.f;

	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	temp1.dmg = Damage(0, 0, 1, 0);
	temp1.radius = 5;
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	temp1.dmg = Damage(0, 0, 0, 1);
	temp1.radius = 10;
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	temp1.dmg = Damage(-2, 0, 0, 0);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	player->currentDeck.deck.push_back(temp1);
	
	player->originalDeck = player->currentDeck;

	player->currentHand.push_back(player->currentDeck.DrawRandom());
	player->currentHand.push_back(player->currentDeck.DrawRandom());
	player->currentHand.push_back(player->currentDeck.DrawRandom());

	player->GO->pos = { m_worldWidth / 2.f, m_worldHeight / 2.f, 1 };

	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.MOVE_FORWARD     )] = new KbKey(Application::GetInstance().usrsttngs.MOVE_FORWARD);
	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.MOVE_BACKWARD    )] = new KbKey(Application::GetInstance().usrsttngs.MOVE_BACKWARD);
	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.MOVE_LEFT        )] = new KbKey(Application::GetInstance().usrsttngs.MOVE_LEFT);
	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.MOVE_RIGHT       )] = new KbKey(Application::GetInstance().usrsttngs.MOVE_RIGHT);
	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.SELECT_CARD_LEFT )] = new KbKey(Application::GetInstance().usrsttngs.SELECT_CARD_LEFT);
	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.SELECT_CARD_RIGHT)] = new KbKey(Application::GetInstance().usrsttngs.SELECT_CARD_RIGHT);
	ih.KeyStorage[unsigned short(Application::GetInstance().usrsttngs.RESTOCK_DECK     )] = new KbKey(Application::GetInstance().usrsttngs.RESTOCK_DECK);
	
	whenToSpawn.StartTimer();
}

double RoundOff(double N, double n)
{
	int h;
	double b, c, d, e, i, j, m, f;
	b = N;
	c = floor(N);

	for (i = 0; b >= 1; ++i)
		b = b / 10;

	d = n - i;
	b = N;
	b = b * pow(10, d);
	e = b + 0.5;
	if ((float)e == (float)ceil(b)) {
		f = (ceil(b));
		h = int(f - 2);
		if (h % 2 != 0) {
			e = e - 1;
		}
	}
	j = floor(e);
	m = pow(10, d);
	j = j / m;
	return j;
}

void SceneGame::Update(double dt_raw)
{
	double dt = m_speed * dt_raw;

	//Physics Simulation Section
	fps = (float)(1.f / dt_raw);

	//Keyboard Section
	if(Application::GetInstance().IsKeyPressed('1') && Application::GetInstance().IsKeyPressed(VK_SHIFT))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::GetInstance().IsKeyPressed('2') && Application::GetInstance().IsKeyPressed(VK_SHIFT))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		float temp = (float)RoundOff((double)axes[2], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			if (Application::GetInstance().usrsttngs.cursorInvX) temp = -temp;
			cursorGO->pos.x += (float)dt * temp * Application::GetInstance().usrsttngs.cursorSensX;
			skipPollMouse = true;
		}
		temp = (float)RoundOff((double)axes[3], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			if (Application::GetInstance().usrsttngs.cursorInvY) temp = -temp;
			cursorGO->pos.y -= (float)dt * temp * Application::GetInstance().usrsttngs.cursorSensY;
			skipPollMouse = true;
		}
	}

	//Mouse Section
	double a, b;
	Application::GetInstance().GetCursorPos(&a, &b);
	if (vec3((float)a, (float)b, 1) != vec3(clickpos.x, clickpos.y, 1))
	{
		clickpos = vec3((float)a, (float)b);
		skipPollMouse = false;
	}

	if (!skipPollMouse)
	{
		float x = (2.0f * (float)a) / (float)Application::GetInstance().GetWindowWidth() - 1.0f;
		float y = 1.0f - (2.0f * (float)b) / (float)Application::GetInstance().GetWindowHeight();
		float z = 1.0f;
		cursorGO->pos = vec3(x, y, z);

		mat4 InvProj, InvView;
		InvProj = projectionStack.Top();
		InvProj = InvProj.GetInverse();

		InvView.SetToLookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);

		InvView = InvView.GetInverse();

		cursorGO->pos = InvProj * cursorGO->pos;
		cursorGO->pos = InvView * cursorGO->pos;
		cursorGO->pos += camera.position;
	}

	bool SkipKBXDirInput = false;
	bool SkipKBYDirInput = false;

	// poll controller first
	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		float temp = (float)RoundOff((double)axes[0], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			SkipKBXDirInput = true;
			player->MoveX_Pad(temp, dt);
		}
		temp = (float)RoundOff((double)axes[1], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			SkipKBYDirInput = true;
			player->MoveY_Pad(temp, dt);
		}
	}

	//then poll kb
	if (!SkipKBYDirInput)
	{
		if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.MOVE_FORWARD, true))
			player->MoveY_KB(1, dt);
		if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.MOVE_BACKWARD, true))
			player->MoveY_KB(0, dt);
	}
	if (!SkipKBXDirInput)
	{
		if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.MOVE_RIGHT, true))
			player->MoveX_KB(1, dt);
		if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.MOVE_LEFT, true))
			player->MoveX_KB(0, dt);
	}

	if (!SkipKBXDirInput && !SkipKBYDirInput)
	{
		player->GO->sprites["MOVE_X_LEFT"].first->m_anim->Set(0, 3, 0.5f, true, true);
		player->GO->sprites["MOVE_X_RIGHT"].first->m_anim->Set(0, 3, 0.5f, true, true);
	}

	// used to keep the cursor stable
	cursorGO->pos += player->GO->vel * (float)dt;

	//TODO: FIND A WAY FOR CONTROLLER ANIMATION TIME DILATION (BASICALLY SLOWER MOVEMENT = BIGGER TIME NUMBER)

	player->GO->activeSprite = player->GO->sprites["IDLE"];

	if (player->GO->vel.y > 0)
		player->GO->activeSprite = player->GO->sprites["MOVE_X_LEFT"];
	else if (player->GO->vel.y < 0)
		player->GO->activeSprite = player->GO->sprites["MOVE_X_RIGHT"];
	
	if (player->GO->vel.x > 0)
		player->GO->activeSprite = player->GO->sprites["MOVE_X_RIGHT"];
	else if (player->GO->vel.x < 0)
		player->GO->activeSprite = player->GO->sprites["MOVE_X_LEFT"];

	player->GO->scale;


	if (skipPollMouse)
		glfwSetInputMode(Application::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(Application::GetInstance().GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	if (Application::GetInstance().scrollState == 1)
	{
		SelectCard(1);
	}
	else if (Application::GetInstance().scrollState == -1)
	{
		SelectCard(0);
	}

	if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.SELECT_CARD_LEFT))
	{
		SelectCard(1);
	}

	if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.SELECT_CARD_RIGHT))
	{
		SelectCard(0);
	}
	if (ih.GetKeyPressed(Application::GetInstance().usrsttngs.RESTOCK_DECK))
	{
		ReloadDeck();
	}

	static bool DoSlowMo = false;
	static bool ControllerDoSlowMo = false;
	if (Application::GetInstance().IsKeyPressed('F'))
	{
		DoSlowMo = true;
	}
	else
	{
		if (!ControllerDoSlowMo)
			DoSlowMo = false;
	}

	if (present == 1)
	{
		int count;
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
		static bool RTPressed = false;
		if (!RTPressed && buttons[7] == GLFW_PRESS)
		{
			RTPressed = true;
			cursorGO->activeSprite = cursorGO->sprites["CLICK"];
			FireCard();
		}
		else if (RTPressed && buttons[7] == GLFW_RELEASE)
		{
			RTPressed = false;
			cursorGO->activeSprite = cursorGO->sprites["NORMAL"];
		}

		static bool LBPressed = false;
		if (!LBPressed && buttons[4] == GLFW_PRESS)
		{
			LBPressed = true;
			SelectCard(1);
		}
		else if (LBPressed && buttons[4] == GLFW_RELEASE)
		{
			LBPressed = false;
		}

		static bool RBPressed = false;
		if (!RBPressed && buttons[5] == GLFW_PRESS)
		{
			RBPressed = true;
			SelectCard(0);
		}
		else if (RBPressed && buttons[5] == GLFW_RELEASE)
		{
			RBPressed = false;
		}

		static bool PadXPressed = false;
		if (!PadXPressed && buttons[3] == GLFW_PRESS)
		{
			PadXPressed = true;
			ReloadDeck();
		}
		else if (PadXPressed && buttons[3] == GLFW_RELEASE)
		{
			PadXPressed = false;
		}

		static bool LTPressed = false;
		if (!LTPressed && buttons[6] == GLFW_PRESS)
		{
			LTPressed = true;
			DoSlowMo = ControllerDoSlowMo = true;
		}
		else if (LTPressed && buttons[6] == GLFW_RELEASE)
		{
			LTPressed = false;
			DoSlowMo = ControllerDoSlowMo = false;
		}
	}

	static bool bLButtonState = false;
	if (!bLButtonState && Application::GetInstance().IsMousePressed(0))
	{
		bLButtonState = true;
		cursorGO->activeSprite = cursorGO->sprites["CLICK"];
		FireCard();
	}
	else if(bLButtonState && !Application::GetInstance().IsMousePressed(0))
	{
		bLButtonState = false;
		cursorGO->activeSprite = cursorGO->sprites["NORMAL"];
	}
	
	static bool bRButtonState = false;
	if(!bRButtonState && Application::GetInstance().IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if(bRButtonState && !Application::GetInstance().IsMousePressed(1))
	{
		bRButtonState = false;
	}


	if (DoSlowMo && slowMoTimer > 0.f)
	{
		m_speed = 0.5f;
		slowMoTimer = Math::Max(float(slowMoTimer - 1.f * dt), (float)0);
	}
	else
	{
		m_speed = 1.0f;
		slowMoTimer = Math::Min(float(slowMoTimer + 0.1f * dt), (float)3);
	}

	if (whenToSpawn.Lap() > 3)
	{
		whenToSpawn.StartTimer();

		enemies.push_back(new Enemy());
		enemies.back()->Init(GOMan->FetchGO());
		enemies.back()->GO->pos.z = 4;
		enemies.back()->GO->sprites["IDLE"].first = MeshBuilder::GenerateSpriteAnimation("a", 1, 1);
		enemies.back()->GO->sprites["IDLE"].second = Load::TGA("Image//range.tga");
		enemies.back()->GO->activeSprite = enemies[0]->GO->sprites["IDLE"];
		enemies.back()->GO->scale = 5;
		enemies.back()->personalDmg = { 168, 0, 0, 0 };
		enemies.back()->maxSpeed = 5.f;
		enemies.back()->speed = 20.f;
		enemies.back()->health = 1.f;
		enemies.back()->maxHealth = 1.f;
		enemies.back()->resistance = { 0, 0, 0, 0 };
		enemies.back()->t.StartTimer();
	}

	/*==============================================\
	|              ENTITY UPDATE LOOPS              |
	\==============================================*/

	player->GO->activeSprite.first->Update(dt);

	for (size_t i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->GO->activeSprite.first->Update(dt);

		std::setprecision(2);
		if ((projectiles[i]->GO->pos - projectiles[i]->targetArea).Length() < 1.f)
		{
			projectiles[i]->spellToCastAtArea.Init(GOMan->FetchGO());
			projectiles[i]->GO->active = false;
			projectiles[i]->spellToCastAtArea.DropAt(projectiles[i]->targetArea);
			projectiles[i]->GO->vel.SetZero();
			Spell* spell = new Spell();
			*spell = projectiles[i]->spellToCastAtArea;
			spell->timer.StartTimer();
			spell->GO->scale = spell->radius * 2.f;
			spell->GO->sprites["RANGE"] = BaseSpell;
			spell->GO->activeSprite = spell->GO->sprites["RANGE"];
			spell->GO->transparency = 0.45f;
			spells.push_back(spell);
			delete projectiles[i];
			projectiles[i] = nullptr;
			projectiles.erase(projectiles.begin() + i);
			--i;
		}
		std::setprecision(6);
	}

	for (size_t i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i]->markedForDeletion)
		{
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
			continue;
		}

		//std::cout << "enemy hp: " << enemies[i]->health << '\n';

		if (CircleCollision(player, enemies[i]))
		{
			if (enemies[i]->t.Lap() > 1)
			{
				player->TakeDamage(enemies[i]->personalDmg, dt);
				enemies[i]->t.StartTimer();
				if (!enemies[i]->GO->vel.IsZero())
					enemies[i]->GO->vel -= 100.f * (float)dt;
			}
		}
		else
		{
			enemies[i]->GO->vel += (player->GO->pos - enemies[i]->GO->pos).Normalized() * enemies[i]->speed * (float)dt;
			enemies[i]->GO->vel.z = 0;
			enemies[i]->GO->vel.x = min(enemies[i]->GO->vel.x, enemies[i]->maxSpeed);
			enemies[i]->GO->vel.y = min(enemies[i]->GO->vel.y, enemies[i]->maxSpeed);
		}

		if (enemies[i]->health <= 0.0f)
		{
			enemies[i]->GO->active = false;
			enemies[i]->markedForDeletion = true;
		}
	}

	for (size_t i = 0; i < spells.size(); ++i)
	{
		if (spells[i]->timer.Lap() >= spells[i]->delay)
		{
			spells[i]->DamageNearby(player, dt);
		}
		for (size_t j = 0; j < enemies.size(); ++j)
		{
			if (spells[i]->timer.Lap() >= spells[i]->delay)
			{
				if (!enemies[j]->markedForDeletion)
					spells[i]->DamageNearby(enemies[j], dt);
			}
		}

		if (spells[i]->timer.Lap() >= spells[i]->duration + spells[i]->delay)
		{
			spells[i]->GO->active = false;
			delete spells[i];
			spells[i] = nullptr;
			spells.erase(spells.begin() + i);
		}
	}

	for (size_t i = 0; i < GOMan->GOContainer.size(); i++)
	{
		if (!GOMan->GOContainer[i]->active)
		{
			delete GOMan->GOContainer[i];
			GOMan->GOContainer.erase(GOMan->GOContainer.begin() + i);
			--i;
			continue;
		}

		GOMan->GOContainer[i]->pos += GOMan->GOContainer[i]->vel * (float)dt;
		if (!GOMan->GOContainer[i]->vel.IsZero() && !GOMan->GOIsHookedOnByClass(i, Projectile))
		{
			vec3 friction = (0.0075f * GOMan->GOContainer[i]->mass * vec3(0, -9.8f, 0)).Length() * GOMan->GOContainer[i]->vel.Normalized() * (float) dt * 100;

			GOMan->GOContainer[i]->vel.x = Math::Clamp(GOMan->GOContainer[i]->vel.x - friction.x, GOMan->GOContainer[i]->vel.x > 0 ? 0 : GOMan->GOContainer[i]->vel.x, GOMan->GOContainer[i]->vel.x < 0 ? 0 : GOMan->GOContainer[i]->vel.x);
			GOMan->GOContainer[i]->vel.y = Math::Clamp(GOMan->GOContainer[i]->vel.y - friction.y, GOMan->GOContainer[i]->vel.y > 0 ? 0 : GOMan->GOContainer[i]->vel.y, GOMan->GOContainer[i]->vel.y < 0 ? 0 : GOMan->GOContainer[i]->vel.y);
		}
	}

	// HACK: THIS IS ASSUMING THAT PLAYER IS ALWAYS THERE
	player->cameraAttachment->position = player->GO->pos + vec3(0, 0, 1);
	player->cameraAttachment->target = player->GO->pos;

}

void SceneGame::FireCard()
{
	if (player->currentHand.size() > 0)
	{
		Projectile* p = new Projectile();
		p->Init(GOMan->FetchGO());
		p->GO->sprites["MOVING"] = BaseProjectile;
		p->GO->activeSprite = p->GO->sprites["MOVING"];
		p->GO->pos = player->GO->pos;
		p->GO->pos.z = 1;
		p->GO->vel = (cursorGO->pos - player->GO->pos).Normalized() * player->yeetSpeed;
		p->GO->vel.z = 0;
		p->GO->scale = 3;
		p->targetArea.x = cursorGO->pos.x;
		p->targetArea.y = cursorGO->pos.y;
		p->targetArea.z = 1;
		p->spellToCastAtArea = player->currentHand[selectedCard - 1];
		projectiles.push_back(p);

		player->currentHand.erase(player->currentHand.begin() + selectedCard - 1);
		if (player->currentDeck.deck.size() > 0)
		{
			player->currentHand.push_back(player->currentDeck.DrawRandom());
		}

		if (player->currentHand.size() == 2 && selectedCard == 3)
		{
			selectedCard = 2;
		}
		else if (player->currentHand.size() == 1 && selectedCard >= 2)
		{
			selectedCard = 1;
		}
	}
}

void SceneGame::SelectCard(bool up)
{
	selectedCard = Math::Wrap((unsigned short)(selectedCard + 1 - 2 * up), (unsigned short)1, (unsigned short)((unsigned short)player->currentHand.size() > 0 ? (unsigned short)player->currentHand.size() : 1));
}

void SceneGame::ReloadDeck()
{
	player->currentDeck = player->originalDeck;

	player->currentHand.push_back(player->currentDeck.DrawRandom());
	player->currentHand.push_back(player->currentDeck.DrawRandom());
	player->currentHand.push_back(player->currentDeck.DrawRandom());
}

void SceneGame::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	defaultShader.SetBool("textEnabled", true);
	defaultShader.SetBool("lightEnabled", false);
	defaultShader.SetBool("colorTextureEnabled", true);
	defaultShader.SetVec3("textColor", { color.r,color.g,color.b });
	defaultShader.SetInt ("colorTexture", 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		defaultShader.SetMat4("MVP", MVP);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	defaultShader.SetBool("textEnabled", false);
}

void SceneGame::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	defaultShader.SetMat4("MVP", MVP);
	if(enableLight && bLightEnabled)
	{
		defaultShader.SetBool("lightEnabled", true);
		modelView = viewStack.Top() * modelStack.Top();
		defaultShader.SetMat4("MV", modelView);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		defaultShader.SetMat4("MV_inverse_transpose", modelView_inverse_transpose);
		
		//load material
		defaultShader.SetVec3("material.kAmbient", {mesh->material.kAmbient.r, mesh->material.kAmbient.g, mesh->material.kAmbient.b, });
		defaultShader.SetVec3("material.kDiffuse", {mesh->material.kDiffuse.r, mesh->material.kDiffuse.g, mesh->material.kDiffuse.b });
		defaultShader.SetVec3("material.kSpecular", {mesh->material.kSpecular.r, mesh->material.kSpecular.g , mesh->material.kSpecular.b });
		defaultShader.SetFloat("material.kShininess", mesh->material.kShininess);
	}
	else
	{	
		defaultShader.SetBool("lightEnabled", false);
	}
	if (mesh->textureID > 0)
	{
		defaultShader.SetFloat("transparency", mesh->transparency);
		defaultShader.SetBool("colorTextureEnabled", true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		defaultShader.SetInt("colorTexture", 0);
	}
	else
	{
		defaultShader.SetBool("colorTextureEnabled", false);
	}
	mesh->Render();
	defaultShader.SetFloat("transparency", 0);
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 projection;

	projection.SetToOrtho(
		-40,
		 40,
		-40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(),
		 40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(),
		-10,
		 10
	);
	projectionStack.LoadMatrix(projection);
	
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);
	
	for (size_t i = 0; i < GOMan->GOContainer.size(); i++)
	{
		GameObject ColliderDisplay;
		ColliderDisplay.sprites["circle"].first = blankSprite;
		ColliderDisplay.sprites["circle"].second = BaseSpell.second;
		ColliderDisplay.sprites["circle"].first->textureID = ColliderDisplay.sprites["circle"].second;

		if (GOMan->GOContainer[i]->active)
		{
			modelStack.PushMatrix();
				modelStack.Translate(GOMan->GOContainer[i]->pos.x, GOMan->GOContainer[i]->pos.y, GOMan->GOContainer[i]->pos.z);
				modelStack.Scale(GOMan->GOContainer[i]->scale, GOMan->GOContainer[i]->scale, GOMan->GOContainer[i]->scale);
				GOMan->GOContainer[i]->activeSprite.first->textureID = GOMan->GOContainer[i]->activeSprite.second;
				RenderMesh(GOMan->GOContainer[i]->activeSprite.first, false);
				RenderMesh(ColliderDisplay.sprites["circle"].first, false);
				GOMan->GOContainer[i]->activeSprite.first->textureID = 0;
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	//=============
	//     UI
	//=============

	modelStack.PushMatrix();
		modelStack.Translate(cursorGO->pos.x, cursorGO->pos.y, 9);
		modelStack.Scale(3.5f, 3.5f, 3.5f);
		cursorGO->activeSprite.first->textureID = cursorGO->activeSprite.second;
		RenderMesh(dynamic_cast<Mesh*>(cursorGO->activeSprite.first), false);
		cursorGO->activeSprite.first->textureID = 0;
	modelStack.PopMatrix();

	std::stringstream fpsCounter;
	fpsCounter << (int)fps;
	std::setprecision(6);

	std::stringstream playerHpCurr;
	playerHpCurr << (int)ceil(player->health);

	std::stringstream playerHpMax;
	playerHpMax << (int)player->maxHealth;

	viewStack.LoadIdentity();
	// Top Left
	modelStack.PushMatrix();
		modelStack.Translate(-40, 40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
		modelStack.PushMatrix();
			modelStack.Translate(2, -2, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], fpsCounter.str() , {1, 1, 1});
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	//// Top Centre
	//modelStack.PushMatrix();
	//	modelStack.Translate(0, 40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
	//	modelStack.PushMatrix();
	//		RenderMesh(meshList[GEO_QUAD], false);
	//	modelStack.PopMatrix();
	//modelStack.PopMatrix();

	//// Top Right
	//modelStack.PushMatrix();
	//	modelStack.Translate(40, 40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
	//	modelStack.PushMatrix();
	//		RenderMesh(meshList[GEO_QUAD], false);
	//	modelStack.PopMatrix();
	//modelStack.PopMatrix();

	//// Bot Right
	//modelStack.PushMatrix();
	//	modelStack.Translate(40, -40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
	//	modelStack.PushMatrix();
	//		RenderMesh(meshList[GEO_QUAD], false);
	//	modelStack.PopMatrix();
	//modelStack.PopMatrix();

	// Bot Centre
	modelStack.PushMatrix();
		modelStack.Translate(0, -40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
		if (player->currentHand.size() > 0)
		{
			std::stringstream ss;
			ss.str("");
			ss << RoundOff(player->currentHand[selectedCard - 1].delay, 3);

			modelStack.PushMatrix();
				modelStack.Translate(0.f, 5.f, 0.f);
				modelStack.Scale(15.f, 4.5f, 1.f);
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_INFO];
				RenderMesh(meshList[GEO_QUAD], false);
				meshList[GEO_QUAD]->textureID = 0;
			modelStack.PopMatrix();
			modelStack.PushMatrix();
				modelStack.Translate(-6.5f, 6.5f, 1.f);
				modelStack.Scale(1.f, 1.f, 1.f);
				RenderText(meshList[GEO_TEXT], "Rad: " + std::to_string(player->currentHand[selectedCard - 1].radius), {1, 1, 1});
			modelStack.PopMatrix();
			modelStack.PushMatrix();
				modelStack.Translate(-6.5f, 5.5f, 1.f);
				modelStack.Scale(1.f, 1.f, 1.f);
				std::setprecision(3);
				RenderText(meshList[GEO_TEXT], "Dly: " + ss.str(), {1, 1, 1});
			modelStack.PopMatrix();
			modelStack.PushMatrix();
				modelStack.Translate(-6.5f, 4.5f, 1.f);
				modelStack.Scale(1.f, 1.f, 1.f);
				ss.str("");
				ss << RoundOff(player->currentHand[selectedCard - 1].duration, 3);
				RenderText(meshList[GEO_TEXT], "Dur: " + ss.str(), {1, 1, 1});
				std::setprecision(6);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
				modelStack.Translate(-6.5f, 3.5f, 1.f);
				modelStack.Scale(1.f, 1.f, 1.f);
				if (player->currentHand[selectedCard - 1].dmg.Phys > 0)
				{
					RenderText(meshList[GEO_TEXT], "Val: " + std::to_string((int)player->currentHand[selectedCard - 1].dmg.Phys), { 1, 1, 1 });
				}
				else if (player->currentHand[selectedCard - 1].dmg.Fire > 0)
				{
					RenderText(meshList[GEO_TEXT], "Val: " + std::to_string((int)player->currentHand[selectedCard - 1].dmg.Fire), { 1, 1, 1 });
				}
				else if (player->currentHand[selectedCard - 1].dmg.Cold > 0)
				{
					RenderText(meshList[GEO_TEXT], "Val: " + std::to_string((int)player->currentHand[selectedCard - 1].dmg.Cold), { 1, 1, 1 });
				}
				else if (player->currentHand[selectedCard - 1].dmg.Light > 0)
				{
					RenderText(meshList[GEO_TEXT], "Val: " + std::to_string((int)player->currentHand[selectedCard - 1].dmg.Light), { 1, 1, 1 });
				}
				else if (player->currentHand[selectedCard - 1].dmg.Total() < 0)
				{
					RenderText(meshList[GEO_TEXT], "Val: " + std::to_string(-(int)player->currentHand[selectedCard - 1].dmg.Total()), { 1, 1, 1 });
				}
			modelStack.PopMatrix();
		}
	modelStack.PopMatrix();

	//// Bot Left
	//modelStack.PushMatrix();
	//	modelStack.Translate(-40, -40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
	//	modelStack.PushMatrix();
	//		RenderMesh(meshList[GEO_QUAD], false);
	//	modelStack.PopMatrix();
	//modelStack.PopMatrix();

	// Mid
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 5);
		// LIFE BAR
		modelStack.PushMatrix();
			modelStack.Translate(-5, 3.5f, 0);
			modelStack.Scale(((player->health) / (player->maxHealth)) * 10, 1, 1);
			meshList[GEO_BAR]->textureID = textures[TEXTURE_LIST::LIFE_BAR_FILL];
			RenderMesh(meshList[GEO_BAR], false);
			meshList[GEO_BAR]->textureID = 0;
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(-5.75f, 3.25f, 1);
			modelStack.Scale(1.5f, 1.5f, 1);
			if (Application::GetInstance().usrsttngs.lifebarDecoration == 0)
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::LIFE_BAR_LEFT];
			else if (Application::GetInstance().usrsttngs.lifebarDecoration == 1)
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::LIFE_BAR_LEFT_ANIMAL];
			RenderMesh(meshList[GEO_QUAD], false);
			meshList[GEO_QUAD]->textureID = 0;
		modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(5.75f, 3.25f, 1);
			modelStack.Scale(1.5f, 1.5f, 1);
			if (Application::GetInstance().usrsttngs.lifebarDecoration == 0)
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::LIFE_BAR_RIGHT];
			else if (Application::GetInstance().usrsttngs.lifebarDecoration == 1)
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::LIFE_BAR_RIGHT_ANIMAL];
			RenderMesh(meshList[GEO_QUAD], false);
			meshList[GEO_QUAD]->textureID = 0;
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(-6.f, 5.f, 0);
			modelStack.Scale(1.5f, 1.5f, 1.f);
			RenderText(meshList[GEO_TEXT], playerHpCurr.str(), { 1, 1, 1 });
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(-6.f, 1.5f, 0);
			modelStack.Scale(1.5f, 1.5f, 1.f);
			RenderText(meshList[GEO_TEXT], playerHpMax.str(), { 1, 1, 1 });
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			modelStack.Translate(-5.f, 2.75f, 0);
			modelStack.Scale(slowMoTimer / 3.f * 10.f, 0.5f, 2);
			meshList[GEO_BAR]->textureID = textures[TEXTURE_LIST::SLOW_BAR];
			RenderMesh(meshList[GEO_BAR], false);
			meshList[GEO_BAR]->textureID = 0;
			modelStack.PopMatrix();

		// CARD BAR
		modelStack.PushMatrix();
			modelStack.Translate(0, -7.5f, 0);
			if (player->currentHand.size() > 0)
			{
				modelStack.PushMatrix();
					if (selectedCard == 1)
						modelStack.Translate(-6, 0, 0);
					else if (selectedCard == 2)
						modelStack.Translate(0, 0, 0);
					else if (selectedCard == 3)
						modelStack.Translate(6, 0, 0);

					modelStack.Scale(5, 5, 1);
					meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_SELECTOR];
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = 0;
				modelStack.PopMatrix();
			}

			if (player->currentHand.size() > 2)
			{
				modelStack.PushMatrix();
					modelStack.Translate(6, 0, 0);
					modelStack.Scale(5, 5, 1);
					if (player->currentHand[2].dmg.Phys > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_PHYS];
					}
					else if (player->currentHand[2].dmg.Fire > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_FIRE];
					}
					else if (player->currentHand[2].dmg.Cold > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_COLD];
					}
					else if (player->currentHand[2].dmg.Light > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_LTNG];
					}
					else if (player->currentHand[2].dmg.Total() < 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_HEAL];
					}
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_BACK];
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = 0;
					modelStack.PopMatrix();
			}
			if (player->currentHand.size() > 1)
			{
				modelStack.PushMatrix();
					modelStack.Translate(0, 0, 0);
					modelStack.Scale(5, 5, 1);
					if (player->currentHand[1].dmg.Phys > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_PHYS];
					}
					else if (player->currentHand[1].dmg.Fire > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_FIRE];
					}
					else if (player->currentHand[1].dmg.Cold > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_COLD];
					}
					else if (player->currentHand[1].dmg.Light > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_LTNG];
					}
					else if (player->currentHand[1].dmg.Total() < 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_HEAL];
					}
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_BACK];
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = 0;
					modelStack.PopMatrix();
			}
			if (player->currentHand.size() > 0)
			{
				modelStack.PushMatrix();
					modelStack.Translate(-6, 0, 0);
					modelStack.Scale(5, 5, 1);
					if (player->currentHand[0].dmg.Phys > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_PHYS];
					}
					else if (player->currentHand[0].dmg.Fire > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_FIRE];
					}
					else if (player->currentHand[0].dmg.Cold > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_COLD];
					}
					else if (player->currentHand[0].dmg.Light > 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_LTNG];
					}
					else if (player->currentHand[0].dmg.Total() < 0)
					{
						meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_ELEMENT_HEAL];
					}
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::CARD_BACK];
					RenderMesh(meshList[GEO_QUAD], false);
					meshList[GEO_QUAD]->textureID = 0;
					modelStack.PopMatrix();
			}
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneGame::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);

	if (cursorGO)
	{
		delete cursorGO;
		cursorGO = nullptr;
	}

	if (player)
	{
		delete player;
		player = nullptr;
	}

	//Cleanup GameObjects
	while (projectiles.size() > 0)
	{
		Projectile* go = projectiles.back();
		if (go)
			delete go;
		projectiles.pop_back();
	}

	while (spells.size() > 0)
	{
		Spell* go = spells.back();
		if (go)
			delete go;
		spells.pop_back();
	}

	while (enemies.size() > 0)
	{
		Enemy* go = enemies.back();
		if (go)
			delete go;
		enemies.pop_back();
	}

	//std::for_each(projectiles.begin(), projectiles.end(), delete_pointer_element<Projectile*>());
	//std::for_each(spells.begin(), spells.end(), delete_pointer_element<Spell*>());

	delete GOMan;
	GOMan = nullptr;

}
