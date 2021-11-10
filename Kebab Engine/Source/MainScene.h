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
	std::vector<GameObject*>& GetGameObjects();
	void DeleteAllGameObjects();

	GameObject* GetGameObjectByUuid(int uuid);
	inline const GameObject* GetRoot() { return root; }

private:
	std::vector<GameObject*> gameObjects;
	GameObject* root;

};