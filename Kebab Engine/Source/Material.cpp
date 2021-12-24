#include "Application.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "Camera3D.h"
#include "ComponentCamera.h"

#include "ResourceManager.h"
#include "Material.h"

#include "Shader.h"
#include "Texture.h"

Material::Material() : Resource(ResourceType::MATERIAL)
{
	name = "New Material";

	shader = app->renderer3D->GetDefaultShader();

	uuid = ResourceManager::GetInstance()->GenerateUUID();

	ambientColor = { 0.4,0.4,0.4 };
}

Material::~Material()
{
}

void Material::Bind(const float4x4& transform)
{
	if (!shader)
		return;

	ComponentCamera* cam = 0;
	switch (app->editor->GetSceneState())
	{
		case SceneState::EDIT:
		{
			cam = app->camera->editorCam;
			break;
		}
		case SceneState::PAUSE:
		case SceneState::PLAY:
		case SceneState::STEP_ONE_FRAME:
		{
			cam = app->camera->gameCam;
			break;
		}
	}
		
	shader->Bind();
	shader->SetUniformMatrix4f("model", transform.Transposed());
	float4x4 view = cam->frustum.ViewMatrix();
	shader->SetUniformMatrix4f("view", view.Transposed());
	shader->SetUniformMatrix4f("projection", cam->frustum.ProjectionMatrix().Transposed());
	float4x4 normalMat = view;
	normalMat.Inverse();
	shader->SetUniformMatrix3f("normalMatrix", normalMat.Float3x3Part().Transposed());

	shader->SetUniformVec3f("lightPos", { 10,20,0 });

	shader->SetUniform1f("shininess", shininess);
	shader->SetUniformVec3f("ambientColor", ambientColor);

}

void Material::Unbind()
{
	shader->Unbind();
}
