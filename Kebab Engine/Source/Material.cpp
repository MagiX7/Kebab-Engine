#include "Material.h"

#include "Texture.h"

Material::Material()
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