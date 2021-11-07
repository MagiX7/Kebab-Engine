#pragma once
#include "Module.h"

#include "Globals.h"

#include <vector>

#define BOUNCER_TIME 200

class GameObject;

class MainScene : public Module
{
public:
	MainScene(bool startEnabled = true);
	~MainScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void AddGameObject(GameObject* go);
	void DeleteGameObject(GameObject* go);
	inline std::vector<GameObject*>& GetGameObjects() { return gameObjects; }
	void DeleteAllGameObjects();

private:
	std::vector<GameObject*> gameObjects;

};