#pragma once

#include "parson.h"

#include <string> // to_string used in save function

class Application;

class Module
{
private :
	bool enabled;

public:
	Application* App;

	Module(Application* parent, bool startEnabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
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

	virtual JSON_Object* Save(JSON_Object* root)
	{
		return NULL;
	}

	virtual void Load()
	{

	}

public:
	std::string name;
};