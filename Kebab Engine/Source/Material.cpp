#include "Application.h"
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

	shader->Bind();
	shader->SetUniformMatrix4f("model", transform.Transposed());
	float4x4 view = app->camera->gameCam->frustum.ViewMatrix();
	shader->SetUniformMatrix4f("view", view.Transposed());
	shader->SetUniformMatrix4f("projection", app->camera->gameCam->frustum.ProjectionMatrix());

}

void Material::Unbind()
{
	shader->Unbind();
}

void Material::SetTexture(Texture* tex)
{
}