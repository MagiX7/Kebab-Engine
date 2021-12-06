#pragma once
#include "Module.h"

#include "GameObject.h"
#include "ComponentCamera.h"

#include "Globals.h"

#include <vector>

//#define BOUNCER_TIME 200

class GameObject;
class QdTree;

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
	void EraseGameObject(GameObject* go);
	inline std::vector<GameObject*>& GetGameObjects() { return root->GetChilds(); }
	void DeleteAllGameObjects();

	inline GameObject* GetRoot() { return root; }
	GameObject* GetGameObjectByUuid(int uuid);

	ComponentCamera* GetCamera() { return camera; }

	QdTree* rootQT;
public:
	ComponentCamera* camera;

private:
	GameObject* root;
};