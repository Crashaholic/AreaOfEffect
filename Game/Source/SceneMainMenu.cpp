#include "gpch.h"
#include "SceneMainMenu.h"
#include "Application.h"
#include "Handlers/SceneHandler.h"
#include <iomanip>
#include "SceneGame.h"

int regionhit(int x, int y, int w, int h)
{
	double mx = 0.0, my = 0.0;
	Application::GetInstance().GetCursorPos(&mx, &my);
	mx -= Application::GetInstance().GetWindowWidth() / 2.f;
	my -= Application::GetInstance().GetWindowHeight() / 2.f;
	if (mx < x ||
		my < y ||
		mx >= x + w ||
		my >= y + h)
		return 0;
	return 1;
}

#define TOP_LEFT_ANCHOR 0
#define TOP_MID_ANCHOR 1
#define TOP_RIGHT_ANCHOR 2
#define BOT_LEFT_ANCHOR 3
#define BOT_MID_ANCHOR 4
#define BOT_RIGHT_ANCHOR 5
#define MIDDLE_ANCHOR 6

int SceneMainMenu::Button(MenuItem m, std::string text)
{
	MenuItem Processed;
	Processed.x = m.x;
	Processed.y = m.y;
	Processed.w = m.w;
	Processed.h = m.h;
	switch (m.AnchorPoint)
	{
	case TOP_LEFT_ANCHOR:
	{
		Processed.x -= Application::GetInstance().GetWindowWidth() / 2.f;
		Processed.y += Application::GetInstance().GetWindowHeight() / 2.f;
		break;
	}
	case TOP_MID_ANCHOR:
	{
		Processed.y += Application::GetInstance().GetWindowHeight() / 2.f;
		break;
	}
	case TOP_RIGHT_ANCHOR:
	{
		Processed.x += Application::GetInstance().GetWindowWidth() / 2.f;
		Processed.y += Application::GetInstance().GetWindowHeight() / 2.f;
		break;
	}
	case BOT_LEFT_ANCHOR:
	{
		Processed.x -= Application::GetInstance().GetWindowWidth() / 2.f;
		Processed.y -= Application::GetInstance().GetWindowHeight() / 2.f;
		break;
	}
	case BOT_MID_ANCHOR:
	{
		Processed.y -= Application::GetInstance().GetWindowHeight() / 2.f;
		break;
	}
	case BOT_RIGHT_ANCHOR:
	{
		Processed.x += Application::GetInstance().GetWindowWidth() / 2.f;
		Processed.y -= Application::GetInstance().GetWindowHeight() / 2.f;
		break;
	}
	case MIDDLE_ANCHOR:
	default:
	{
		break;
	}
	}
	Processed.x -= Processed.w / 2.f;
	Processed.y -= Processed.h / 2.f;
	//std::cout <<
	//	Processed.x << ',' <<
	//	Processed.y << ',' <<
	//	Processed.w << ',' <<
	//	Processed.h << '\n';
	if (regionhit(Processed.x, Processed.y, m.w, m.h))
	{
		hotitem = text;
		if (activeitem.empty() && Application::GetInstance().IsMousePressed(0))
		{
			activeitem = text;
			std::cout << "AH!\n";
		}
		//std::cout << "Hot: " << hotitem << '\n';
	}

	modelStack.PushMatrix();
	switch (m.AnchorPoint)
	{
	case TOP_LEFT_ANCHOR:
	{
		modelStack.Translate(-(float)Application::GetInstance().GetWindowWidth() / 2.f, (float)Application::GetInstance().GetWindowHeight() / 2.f, 5);
		break;
	}
	case TOP_MID_ANCHOR:
	{
		modelStack.Translate(0, (float)Application::GetInstance().GetWindowHeight() / 2.f, 5);
		break;
	}
	case TOP_RIGHT_ANCHOR:
	{
		modelStack.Translate((float)Application::GetInstance().GetWindowWidth() / 2.f, (float)Application::GetInstance().GetWindowHeight() / 2.f, 5);
		break;
	}
	case BOT_LEFT_ANCHOR:
	{
		modelStack.Translate(-(float)Application::GetInstance().GetWindowWidth() / 2.f, -(float)Application::GetInstance().GetWindowHeight() / 2.f, 5);
		break;
	}
	case BOT_MID_ANCHOR:
	{
		modelStack.Translate(0, -(float)Application::GetInstance().GetWindowHeight() / 2.f, 5);
		break;
	}
	case BOT_RIGHT_ANCHOR:
	{
		modelStack.Translate((float)Application::GetInstance().GetWindowWidth() / 2.f, (float)Application::GetInstance().GetWindowHeight() / 2.f, 5);
		break;
	}
	case MIDDLE_ANCHOR:
	default:
	{
		modelStack.Translate(0, 0, 5);
		break;
	}
	}

	if (hotitem == text)
	{
		if (activeitem == text)
		{
			modelStack.PushMatrix();
				modelStack.Translate(m.x, m.y, 0);
				modelStack.PushMatrix();
				modelStack.Scale(10, 10, 1);
					RenderText(meshList[GEO_TEXT], text, {0, 0, 0});
				modelStack.PopMatrix();
				modelStack.Scale(m.w, m.h, 1);
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::BUTTON];
				RenderMesh(meshList[GEO_QUAD], false);
				meshList[GEO_QUAD]->textureID = 0;
			modelStack.PopMatrix();
		}
		else
		{
			modelStack.PushMatrix();
				modelStack.Translate(m.x, m.y, 0);
				modelStack.PushMatrix();
				modelStack.Scale(10, 10, 1);
					RenderText(meshList[GEO_TEXT], text, { 0, 0, 0 });
				modelStack.PopMatrix();
				modelStack.Scale(m.w, m.h, 1);
				meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::BUTTON];
				RenderMesh(meshList[GEO_QUAD], false);
				meshList[GEO_QUAD]->textureID = 0;
			modelStack.PopMatrix();
		}
	}
	else
	{
		modelStack.PushMatrix();
			modelStack.Translate(m.x, m.y, 0);
			modelStack.PushMatrix();
			modelStack.Scale(10, 10, 1);
				RenderText(meshList[GEO_TEXT], text, { 0, 0, 0 });
			modelStack.PopMatrix();
			modelStack.Scale(m.w, m.h, 1);
			meshList[GEO_QUAD]->textureID = textures[TEXTURE_LIST::BUTTON];
			RenderMesh(meshList[GEO_QUAD], false);
			meshList[GEO_QUAD]->textureID = 0;
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	if (!Application::GetInstance().IsMousePressed(0) &&
		hotitem == text &&
		activeitem == text)
		return 1;

	return 0;
}

SceneMainMenu::SceneMainMenu()
{
}

SceneMainMenu::~SceneMainMenu()
{
}

void SceneMainMenu::Init()
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

	defaultShader.Init("Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	defaultShader.Use();

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1);
	meshList[GEO_BAR] = MeshBuilder::GenerateBar("bar", Color(1, 1, 1), 1);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//couriernew.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	//meshList[GEO_QUAD]->transparency = 0.5f;
	//meshList[GEO_BAR]->transparency = 0.5f;

	textures[TEXTURE_LIST::LOGO  ] = Load::TGA("Image//default_missing.tga");
	textures[TEXTURE_LIST::BUTTON] = Load::TGA("Image//menu_button.tga");

}

void SceneMainMenu::Update(double dt)
{
	//Physics Simulation Section
	fps = (float)(1.f / dt);


}

void SceneMainMenu::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	defaultShader.SetBool("textEnabled", true);
	defaultShader.SetBool("lightEnabled", false);
	defaultShader.SetBool("colorTextureEnabled", true);
	defaultShader.SetVec3("textColor", { color.r,color.g,color.b });
	defaultShader.SetInt("colorTexture", 0);
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

void SceneMainMenu::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	defaultShader.SetMat4("MVP", MVP);
	if (enableLight && bLightEnabled)
	{
		defaultShader.SetBool("lightEnabled", true);
		modelView = viewStack.Top() * modelStack.Top();
		defaultShader.SetMat4("MV", modelView);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		defaultShader.SetMat4("MV_inverse_transpose", modelView_inverse_transpose);

		//load material
		defaultShader.SetVec3("material.kAmbient", { mesh->material.kAmbient.r, mesh->material.kAmbient.g, mesh->material.kAmbient.b, });
		defaultShader.SetVec3("material.kDiffuse", { mesh->material.kDiffuse.r, mesh->material.kDiffuse.g, mesh->material.kDiffuse.b });
		defaultShader.SetVec3("material.kSpecular", { mesh->material.kSpecular.r, mesh->material.kSpecular.g , mesh->material.kSpecular.b });
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
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneMainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 projection;

	projection.SetToOrtho(
		-Application::GetInstance().GetWindowWidth() / 2.f,
		 Application::GetInstance().GetWindowWidth() / 2.f,
		-Application::GetInstance().GetWindowHeight() / 2.f,
		 Application::GetInstance().GetWindowHeight() / 2.f,
		-10,
		10
	);
	//projection.SetToOrtho(
	//	0,
	//	Application::GetInstance().GetWindowWidth(),
	//	Application::GetInstance().GetWindowHeight(),
	//	0,
	//	-10,
	//	10
	//);
	projectionStack.LoadMatrix(projection);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();
	viewStack.LoadIdentity();
	//=============
	//     UI
	//=============

	//modelStack.PushMatrix();
	//	modelStack.Translate(0, 0, 0);
	//	modelStack.Scale(1270, 710, 1);
	//	RenderMesh(meshList[GEO_QUAD], false);
	//modelStack.PopMatrix();

	if (Button(MenuItem(0, 0, 100, 100, MIDDLE_ANCHOR), "Play"))
	{
		SceneHandler::GetInstance()->ChangeScene<SceneGame>();
	}

	std::stringstream fpsCounter;
	fpsCounter << (int)fps;
	std::setprecision(6);


}

void SceneMainMenu::Exit()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);

}
