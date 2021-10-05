#pragma once

#include "parson.h"

#include <string> // to_string used in save function

class Application;

class Module
{
private :
	bool enabled;

public:
	//Application* App;

	Module(bool startEnabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* root) 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw(float dt)
	{
		return true;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void Save(JSON_Object* root)
	{

	}

	virtual void Load(JSON_Object* root)
	{

	}

public:
	std::string name;
};