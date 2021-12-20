#include "Application.h"
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
	shader = new Shader("Assets/Resources/Shaders/basic.shader");
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
	shader->SetUniformMatrix4f("normalMatrix", normalMat.Transposed().Float3x3Part());

}

void Material::Unbind()
{
	shader->Unbind();
}

void Material::SetTexture(Texture* tex)
{
}