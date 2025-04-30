#pragma once
#include "Skateboard/Scene/Scene.h"
#include "Systems/BaseSystem.h"
#include "Scenes/SceneData.h"
#include "StateNames.h"
#include <Skateboard.h>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "Utils/MusicManager.h"

class BaseSystem;
class MainLayer;

class GameScene : public Skateboard::Scene {
private:
	std::vector<std::unique_ptr<BaseSystem>> m_updatingSystems;
	std::vector<std::thread> systemThreads;

	void HandleParallelisedSystems();
	bool stopParallelisedSystems = false;
	Skateboard::TimeManager* updateTime = nullptr;
protected:
	int claimedSystems;
	std::atomic<int> updatedSystems;
	std::mutex updateSystemsMutex;
	std::condition_variable frameChangeCv;
	std::mutex frameChangeMutex;
	bool startSystemsUpdate = false;
	std::condition_variable finishSystemUpdate;
	std::mutex finishSystemsUpdateMutex;

	StateNames SceneChange = StateNames::MainMenu;
	bool SceneChangeQueued = false;

	MainLayer* Layer;

	friend class MainLayer;

	StateNames stateName;

	void RegisterSystem(std::unique_ptr<BaseSystem> system);

	void InitSystems();
	void HandleInputSystems(Skateboard::TimeManager* time);
	void UpdateSystems(Skateboard::TimeManager* time);
	void OnImGuiRenderSystems();
	void OnCollisionSystems(Skateboard::Entity* ent1, Skateboard::Entity* ent2);
public:
	SceneData data;
	MusicCategory music = MENU_MUSIC;

public:
	explicit GameScene(const std::string& name, MainLayer* layer);
	~GameScene();

	virtual void OnRender() override;
	virtual void OnCollision(Skateboard::Entity* ent1, Skateboard::Entity* ent2) = 0;
	/* Called before OnSceneExit, this should not be clearing any memory up or anything, mainly used to update scene data */
	virtual void OnPreSceneExit() {};

	void QueueSceneChange(StateNames state) {
		SceneChange = state;
		SceneChangeQueued = true;
	}

};