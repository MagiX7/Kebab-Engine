#include "Material.h"

#include "Texture.h"

Material::Material() : Resource(ResourceType::MATERIAL)
{
}

Material::~Material()
{
}

void Material::SetTexture(Texture* tex)
{
	if (texture) delete texture;
	texture = tex;
}