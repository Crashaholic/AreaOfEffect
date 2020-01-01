#include "gpch.h"
#include "SceneMainMenu.h"
#include "Application.h"

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
	// Top Centre
	modelStack.PushMatrix();
		modelStack.Translate(0, 40 / (float)Application::GetInstance().GetWindowWidth() * (float)Application::GetInstance().GetWindowHeight(), 5);
		modelStack.PushMatrix();
			RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SceneMainMenu::Exit()
{
}
