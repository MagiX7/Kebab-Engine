#pragma once

#include <string>

class Texture;

class Material
{
public:
	Material();
	virtual ~Material();

	void SetTexture(Texture* tex);

	inline void SetName(const std::string& newName) { name = newName; }
	inline const std::string& GetName() const { return name; }

private:
	std::string name;

	Texture* texture;
};