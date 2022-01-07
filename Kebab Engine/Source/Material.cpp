#include "Application.h"
#include "Renderer3D.h"
#include "Editor.h"
#include "Camera3D.h"
#include "ComponentCamera.h"

#include "ResourceManager.h"
#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "Timer.h"

Material::Material() : Resource(ResourceType::MATERIAL)
{
	name = "New Material";

	shader = app->renderer3D->GetDefaultShader();

	uuid = ResourceManager::GetInstance()->GenerateUUID();

	ambientColor = { 0.4,0.4,0.4 };
	frequency = 2.0f;
	speed = 0.05f;
	amplitude = 0.2f;
	textureAlpha = 1.0f;
	foamSpeed = 0.0f;
	noiseAmount = 1.0f;
	foamDir = { 0.001,0.0 };

	timer = Timer();
}

Material::~Material()
{
	// Shader is deleted in renderer
}

void Material::Bind(const float4x4& transform, ComponentCamera* cam)
{
	if (!shader)
		return;

	//ComponentCamera* cam = 0;
	//switch (app->editor->GetSceneState())
	//{
	//	case SceneState::EDIT:
	//	{
	//		cam = app->camera->editorCam;
	//		break;
	//	}
	//	case SceneState::PAUSE:
	//	case SceneState::PLAY:
	//	case SceneState::STEP_ONE_FRAME:
	//	{
	//		cam = app->camera->gameCam;
	//		break;
	//	}
	//}

	ComponentCamera* matCam = cam;

	if (cam == nullptr)
		matCam = app->camera->editorCam;

	shader->Bind();
	shader->SetUniformMatrix4f("model", transform.Transposed());
	float4x4 view = matCam->frustum.ViewMatrix();
	shader->SetUniformMatrix4f("view", view.Transposed());
	shader->SetUniformMatrix4f("projection", matCam->frustum.ProjectionMatrix().Transposed());
	float4x4 normalMat = view;
	normalMat.Inverse();
	shader->SetUniformMatrix3f("normalMatrix", normalMat.Float3x3Part().Transposed());

	shader->SetUniformVec3f("lightPos", { 10,20,0 });

	shader->SetUniformVec3f("ambientColor", ambientColor);
	
	shader->SetUniform1f("shininess", shininess);

	shader->SetUniform1i("time", timer.Read());
	shader->SetUniform1f("amplitude", amplitude);
	shader->SetUniform1f("frequency", frequency);
	shader->SetUniform1f("speed", speed);
	shader->SetUniform1f("foamSpeed", foamSpeed);
	shader->SetUniform1f("noiseAmount", noiseAmount);
	shader->SetUnifromVec2f("foamDir", foamDir);

	shader->SetUniform1f("textureAlpha", textureAlpha);
}

void Material::Unbind()
{
	shader->Unbind();
}

void Material::SetShader(Shader* s)
{
	shader = s;
	shader->Refresh();
}
