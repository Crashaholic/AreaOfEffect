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


	//m_worldHeight = 100.f;
	//m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

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
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	bLightEnabled = false;

	m_speed = 1.f;
	p.GO = GOMan.FetchGO();
	p.GO->pos = {m_worldWidth / 2.f, m_worldHeight / 2.f };
	p.cameraAttachment = &camera;
	Math::InitRNG();
}

void SceneGame::Update(double dt)
{
	//Keyboard Section
	if(Application::IsKeyPressed('1') && Application::IsKeyPressed(VK_SHIFT))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('2') && Application::IsKeyPressed(VK_SHIFT))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// poll controller first
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (present == 1)
	{
		int axesCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		p.MoveX_Pad(axes[0], dt);
		p.MoveY_Pad(axes[1], dt);
	}
	//then poll kb
	if (Application::IsKeyPressed('W'))
	{
		//camera.position.y += 10 * dt;
		//camera.target.y += 10 * dt;
		p.MoveY_KB(1, dt);
	}
	if (Application::IsKeyPressed('S'))
	{
		//camera.position.y -= 10 * dt;
		//camera.target.y -= 10 * dt;
		p.MoveY_KB(0, dt);
	}
	if (Application::IsKeyPressed('D'))
	{
		//camera.position.x += 10 * dt;
		//camera.target.x += 10 * dt;
		p.MoveX_KB(1, dt);
	}
	if (Application::IsKeyPressed('A'))
	{
		//camera.position.x -= 10 * dt;
		//camera.target.x -= 10 * dt;
		p.MoveX_KB(0, dt);
	}

	//Mouse Section
	static bool bLButtonState = false;
	//Exercise 10: ghost code here
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		Vector3 clickpos;

		clickpos.x = static_cast<float>(x / static_cast<float>(w) * m_worldWidth);
		clickpos.y = static_cast<float>(m_worldHeight - y / static_cast<float>(h) * m_worldHeight);
		std::cout << clickpos << '\n';
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section
	fps = (float)(1.f / dt);
}

void SceneGame::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
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
	glEnable(GL_DEPTH_TEST);
}

void SceneGame::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	defaultShader.SetBool("textEnabled", true);
	defaultShader.SetBool("lightEnabled", false);
	defaultShader.SetBool("colorTextureEnabled", true);
	defaultShader.SetVec3("textColor", { color.r,color.g,color.b });
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	defaultShader.SetInt("colorTexture", 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		defaultShader.SetMat4("MVP", MVP);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	defaultShader.SetBool("textEnabled", false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
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
		-(float)Application::GetWindowWidth() / 2 / 10,
		 (float)Application::GetWindowWidth() / 2 / 10,
		-(float)Application::GetWindowHeight() / 2 / 10,
		 (float)Application::GetWindowHeight() / 2 / 10,
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
		RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth, m_worldHeight, 0);
		RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	std::stringstream fpsCounter;
	fpsCounter << "FPS: " << std::setprecision(2) << fps;
	std::setprecision(6);

	viewStack.LoadIdentity();
	modelStack.PushMatrix();
		modelStack.Translate(-(float)Application::GetWindowWidth() / 2 / 10, (float)Application::GetWindowHeight() / 2 / 10, 0);
		//modelStack.PushMatrix();
		//	RenderMesh(meshList[GEO_QUAD], false);
		//modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(2, -2, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], fpsCounter.str() , {1, 1, 1});
		modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
		modelStack.Translate(-(float)Application::GetWindowWidth() / 2 / 10, (float)Application::GetWindowHeight() / 2 / 10, 0);
		//modelStack.PushMatrix();
		//	RenderMesh(meshList[GEO_QUAD], false);
		//modelStack.PopMatrix();
		modelStack.PushMatrix();
			modelStack.Translate(2, -2, 0);
			modelStack.Scale(2, 2, 2);
			RenderText(meshList[GEO_TEXT], fpsCounter.str() , {1, 1, 1});
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
