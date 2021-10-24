#pragma once
#include "Module.h"
#include "Globals.h"

#include "GameObject.h"

#define BOUNCER_TIME 200

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
	inline const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }

private:
	std::vector<GameObject*> gameObjects;

};