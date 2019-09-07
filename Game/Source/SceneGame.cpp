#include "gpch.h"
#include "SceneGame.h"
#include "Application.h"
#include <iomanip>

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_worldHeight = 100.f;
	m_worldWidth = 100.f;
	
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
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//couriernew.tga", false);
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	cursorGO.textureID = LoadTGA("Image//cursor.tga");

	bLightEnabled = false;

	m_speed = 1.f;
	p.Init(GOMan.FetchGO());
	p.InitCam(&camera);
	p.GO->mass = 10;
	p.GO->textureID = Load::TGA("Image//cursor.tga");

	Math::InitRNG();
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
		h = f - 2;
		if (h % 2 != 0) {
			e = e - 1;
		}
	}
	j = floor(e);
	m = pow(10, d);
	j = j / m;
	return j;
}

void SceneGame::Update(double dt)
{
	//Keyboard Section
	if(Application::GetInstance().IsKeyPressed('1') && Application::GetInstance().IsKeyPressed(VK_SHIFT))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::GetInstance().IsKeyPressed('2') && Application::GetInstance().IsKeyPressed(VK_SHIFT))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	bool SkipKBXDirInput = false;
	bool SkipKBYDirInput = false;

	// poll controller first
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		float temp = (float)RoundOff((double)axes[0], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			SkipKBXDirInput = true;
			p.MoveX_Pad(temp, dt);
		}
		temp = (float)RoundOff((double)axes[1], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			SkipKBYDirInput = true;
			p.MoveY_Pad(axes[1], dt);
		}
	}
	//then poll kb
	if (!SkipKBYDirInput)
	{
		if (Application::GetInstance().IsKeyPressed(Application::GetInstance().usrsttngs.MOVE_FORWARD))
			p.MoveY_KB(1, dt);
		if (Application::GetInstance().IsKeyPressed(Application::GetInstance().usrsttngs.MOVE_BACKWARD))
			p.MoveY_KB(0, dt);
	}
	if (!SkipKBXDirInput)
	{
		if (Application::GetInstance().IsKeyPressed(Application::GetInstance().usrsttngs.MOVE_RIGHT))
			p.MoveX_KB(1, dt);
		if (Application::GetInstance().IsKeyPressed(Application::GetInstance().usrsttngs.MOVE_LEFT))
			p.MoveX_KB(0, dt);
	}

	static bool doNotPollMouse = false;
	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);

		float temp = (float)RoundOff((double)axes[2], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			cursorGO.pos.x += (float)dt * temp * 10;
			doNotPollMouse = true;
		}
		temp = (float)RoundOff((double)axes[3], 1);
		if (temp > 0.0f || temp < 0.0f)
		{
			cursorGO.pos.y -= (float)dt * temp * 10;
			doNotPollMouse = true;
		}
	}

	//Mouse Section
	double a, b;
	Application::GetInstance().GetCursorPos(&a, &b);
	if (vec3(a, b, 1) != vec3(clickpos.x, clickpos.y, 1))
	{
		clickpos = vec3( a, b );
		std::cout << "hey\n";
		doNotPollMouse = false;
	}

	if (!doNotPollMouse)
	{
		float x = (2.0f * a) / (float)Application::GetInstance().GetWindowWidth() - 1.0f;
		float y = 1.0f - (2.0f * b) / (float)Application::GetInstance().GetWindowHeight();
		float z = 1.0f;
		cursorGO.pos = vec3(x, y, z);

		mat4 InvProj, InvView;
		InvProj.SetToOrtho(
			-(float)Application::GetInstance().GetWindowWidth() / 2 / 10,
			(float)Application::GetInstance().GetWindowWidth() / 2 / 10,
			-(float)Application::GetInstance().GetWindowHeight() / 2 / 10,
			(float)Application::GetInstance().GetWindowHeight() / 2 / 10,
			-10,
			10);

		InvProj = InvProj.GetInverse();

		InvView.SetToLookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
		);

		InvView = InvView.GetInverse();
		
		cursorGO.pos = InvProj * cursorGO.pos;
		cursorGO.pos = InvView * cursorGO.pos;
		cursorGO.pos += camera.position;
	}

	static bool bLButtonState = false;
	if(!bLButtonState && Application::GetInstance().IsMousePressed(0))
	{
		bLButtonState = true;
	}
	else if(bLButtonState && !Application::GetInstance().IsMousePressed(0))
	{
		bLButtonState = false;
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

	//Physics Simulation Section
	fps = (float)(1.f / dt);

	for (size_t i = 0; i < GOMan.GOContainer.size(); i++)
	{
		//GOMan.GOContainer[i]->pos += GOMan.GOContainer[i]->vel * dt;
		if (GOMan.GOIsHookedOnByClass(i, Player)/*isHookedby<Player>(i)*/)
		{
			p.cameraAttachment->position = GOMan.GOContainer[i]->pos + vec3(0, 0, 1);
			p.cameraAttachment->target = GOMan.GOContainer[i]->pos;
		}
	}
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
		//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		defaultShader.SetMat4("MVP", MVP);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
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
	if(mesh->textureID > 0)
	{
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
		-(float)Application::GetInstance().GetWindowWidth() / 2 / 10,
		 (float)Application::GetInstance().GetWindowWidth() / 2 / 10,
		-(float)Application::GetInstance().GetWindowHeight() / 2 / 10,
		 (float)Application::GetInstance().GetWindowHeight() / 2 / 10,
		-10, 
		 10);
	projectionStack.LoadMatrix(projection);
	
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);
	
	modelStack.PushMatrix();
		modelStack.Translate(p.GO->pos.x, p.GO->pos.y, p.GO->pos.z);
		modelStack.Scale(10, 10, 10);
		meshList[GEO_QUAD]->textureID = p.GO->textureID;
		RenderMesh(meshList[GEO_QUAD], false);
		meshList[GEO_QUAD]->textureID = 0;
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	std::stringstream clickedpos;
	clickedpos << std::setprecision(1) << clickpos;
	std::setprecision(6);
	modelStack.PushMatrix();
		modelStack.Translate(cursorGO.pos.x, cursorGO.pos.y, 0);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();



	//=============
	//     UI
	//=============

	std::stringstream fpsCounter;
	fpsCounter << "FPS: " << std::setprecision(2) << fps;
	std::setprecision(6);

	std::stringstream playerPos;
	playerPos << std::setprecision(2) << p.GO->pos;
	std::setprecision(6);

	viewStack.LoadIdentity();
	// Top Left
	modelStack.PushMatrix();
		modelStack.Translate(-(float)Application::GetInstance().GetWindowWidth() / 2 / 10, (float)Application::GetInstance().GetWindowHeight() / 2 / 10, 0);
		//modelStack.PushMatrix();
		//	RenderMesh(meshList[GEO_QUAD], false);
		//modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(2, -2, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], fpsCounter.str() , {1, 1, 1});
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Top Right
	modelStack.PushMatrix();
		modelStack.Translate((float)Application::GetInstance().GetWindowWidth() / 2 / 10, (float)Application::GetInstance().GetWindowHeight() / 2 / 10, 0);
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Bot Right
	modelStack.PushMatrix();
		modelStack.Translate((float)Application::GetInstance().GetWindowWidth() / 2 / 10, -(float)Application::GetInstance().GetWindowHeight() / 2 / 10, 0);
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Bot Left
	modelStack.PushMatrix();
		modelStack.Translate(-(float)Application::GetInstance().GetWindowWidth() / 2 / 10, -(float)Application::GetInstance().GetWindowHeight() / 2 / 10, 0);
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	// Mid
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 0);
		modelStack.PushMatrix();
			modelStack.Translate(0, 5, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], playerPos.str(), { 1, 0, 1 });
		modelStack.PopMatrix();
	modelStack.PopMatrix();


}

void SceneGame::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
