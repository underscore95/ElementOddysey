#include "GameScene.h"
#include <Skateboard/Scene/Components.h>
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Utils/Sprites.h"

void GameScene::HandleParallelisedSystems()
{
	while (true) {
		std::unique_lock<std::mutex> lock(frameChangeMutex);
		frameChangeCv.wait(lock, [&] { return stopParallelisedSystems || startSystemsUpdate; });

		if (stopParallelisedSystems)
			return;

		// todo could this be a function so we can reuse the code for handle input
		if (startSystemsUpdate) {
			int systemToUpdate = -1;
			{
				std::unique_lock<std::mutex> lock(updateSystemsMutex);
				if (claimedSystems + 1 >= m_updatingSystems.size()) {
					// oh no, nothing to update
					// honestly i have no idea why but we get a random deadlock on scene exit without this notify
					// doesn't make sense to me, because the notify happens below too
					finishSystemUpdate.notify_one();
					continue;
				}
				++claimedSystems;
				systemToUpdate = claimedSystems; // claim a system, meaning we update it, and other threads wont
			}

			// wait until we can start updating the system
			while (m_updatingSystems[systemToUpdate]->GetUpdatePriority() > m_updatingSystems[updatedSystems + 1]->GetUpdatePriority()) {} // todo no busy wait

			// ok every system with a lower priority has been updated
			m_updatingSystems[systemToUpdate]->RunParallelUpdate(updateTime, this);

			++updatedSystems;
			if (updatedSystems + 1 >= m_updatingSystems.size()) {
				// we finished this frames update
				finishSystemUpdate.notify_one();
			}

			continue;
		}
	}
}

void GameScene::RegisterSystem(std::unique_ptr<BaseSystem> system)
{
	// We need to figure out where to insert into the list
	for (size_t index = 0; index < m_updatingSystems.size(); ++index) {
		if (m_updatingSystems.at(index)->GetUpdatePriority() > system->GetUpdatePriority()) {
			// New system needs to update first
			m_updatingSystems.insert(m_updatingSystems.begin() + index, std::move(system));
			return;
		}
	}

	// New system needs to update after all existing systems
	m_updatingSystems.push_back(std::move(system));
}

void GameScene::InitSystems()
{
	for (auto& s : m_updatingSystems)
		s->Init(this);
}

void GameScene::HandleInputSystems(Skateboard::TimeManager* time)
{
	for (auto& s : m_updatingSystems)
		s->HandleInput(time, this);
}

void GameScene::UpdateSystems(Skateboard::TimeManager* time)
{
	// Parallelisation seems to add ~10-20k ns of overhead
	// todo: make parallelised updating an opt-in?
#ifndef NDEBUG
	auto now = std::chrono::high_resolution_clock::now();
#endif

	claimedSystems = -1;
	updatedSystems = -1;
	updateTime = time;

	startSystemsUpdate = true;
	frameChangeCv.notify_all();

	for (auto& sys : m_updatingSystems) {
		sys->RunUpdate(time, this);
	}

	std::unique_lock<std::mutex> finishLock(finishSystemsUpdateMutex);
	finishSystemUpdate.wait(finishLock, [&]() { return updatedSystems + 1 >= m_updatingSystems.size(); }); // Wait until the parallel threads finished updating
	startSystemsUpdate = false;

#ifndef NDEBUG
	// Time the frame
	auto end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - now).count();
	//std::cout << dur << " ns\n";
#endif
}

void GameScene::OnImGuiRenderSystems()
{
	for (auto& s : m_updatingSystems) {
		s->OnImGuiRender();
	}
}

void GameScene::OnCollisionSystems(Skateboard::Entity* ent1, Skateboard::Entity* ent2)
{
	for (auto& s : m_updatingSystems)
		s->OnCollision(ent1, ent2);
}

GameScene::GameScene(const std::string& name, MainLayer* layer) : Scene(name), Layer{ layer } {
	auto numThreads = std::thread::hardware_concurrency();
	if (numThreads == 0) {
		std::cerr << "Failed to detect number of cores.\n";
		numThreads = 8; // Possibly not the ideal amount, but at least the game runs
	}

	numThreads = 1; // The game at the moment is very lightweight, with more threads, even if we have enough cores, overhead like context switching exceed the performance gain

	for (int i = 0; i < numThreads; ++i) {
		systemThreads.emplace_back([&] { HandleParallelisedSystems(); });
	}
}

GameScene::~GameScene() {
	stopParallelisedSystems = true;
	frameChangeCv.notify_all();
	for (auto& sysThread : systemThreads) sysThread.join();
}

void GameScene::OnRender()
{
	assert(Sprites::AreAllSpritesAdded());
	assert(Sprites::AreAllSpritesValid());

	Scene::OnRender();

	/*auto spriteGroup = Skateboard::ECS::GetRegistry().view<Skateboard::SpriteComponent>();
	for (auto entity : spriteGroup)
	{
		auto& spriteComponent = spriteGroup.get<Skateboard::SpriteComponent>(entity);
		Skateboard::Renderer2D::DrawSprite(spriteComponent);
	}*/

	for (const auto& spr : Sprites::GetSprites()) {
		auto& spriteComponent = Skateboard::ECS::GetRegistry().get<Skateboard::SpriteComponent>(spr.ent);
		Skateboard::Renderer2D::DrawSprite(spriteComponent);
	}

	auto meshGroup = Skateboard::ECS::GetRegistry().view<Skateboard::StaticMeshComponent, Skateboard::TransformComponent>();
	for (auto& entity : meshGroup)
	{
		auto [meshComponent, transform] = meshGroup.get<Skateboard::StaticMeshComponent, Skateboard::TransformComponent>(entity);

		auto m = Skateboard::AssetManager::GetModel(meshComponent.ModelName);

		Skateboard::Renderer3D::DrawModel(m, transform);
	}
}
