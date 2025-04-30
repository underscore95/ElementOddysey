#include "PlayerJoinSystem.h"
#include <Skateboard.h>
#include "Utils/MusicManager.h"

void PlayerJoinSystem::Init(GameScene* scn)
{
}

void PlayerJoinSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
	for (auto& player : Data.Players) {
		if (Skateboard::Input::IsButtonPressed(Skateboard::GamePadButton_::Pad_Button_Right, player.first)) {
			player.second = Element::Next(player.second);
		}
	}
}

void PlayerJoinSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto controllers = Skateboard::Platform::GetPlatform().GetUserManager()->GetUsersByType(Skateboard::UserType::SKTB_USER_LOGIN_PLAYER);
	std::unordered_map<UserID, Skateboard::User> users;
	for (auto& c : controllers) {
		users.insert(std::pair(c.id, c));
	}

	// Remove old players
	std::vector<UserID> toRemove;
	for (auto& player : Data.Players) {
		auto& user = player.first;
		auto it = users.find(user);
		if (it == users.end()) {
			toRemove.push_back(user);
		}
	}

	for (auto user : toRemove) {
		Data.Players.erase(user);
	}

	// Add new players
	unsigned int playerCount = 0;
	for (auto& user : users) {
		auto it = Data.Players.find(user.first);
		if (it == Data.Players.end()) {
			Data.Players.insert(std::pair(user.first, Element::FIRE));
			Skateboard::AudioRenderer::AddAudioUser(user.second);
			++playerCount;
			if (playerCount >= PlayerJoinSystem::MAX_PLAYERS) break;
		}
	}
}

void PlayerJoinSystem::OnImGuiRender()
{
	auto users = Skateboard::Platform::GetPlatform().GetUserManager()->GetUsersByType(Skateboard::UserType::SKTB_USER_LOGIN_PLAYER);
	unsigned int playerCount = std::min(MAX_PLAYERS, users.size());

	ImGui::PushFont(Skateboard::AssetManager::GetFont("arial_50"));
	std::stringstream ss;
	ss << "Connected Players (" << playerCount << " / " << MAX_PLAYERS << "):";
	std::string players = ss.str();
	ImGui::TextColored(ImVec4(0, 0, 0, 1), players.c_str());

	unsigned int playerNumber = 1;
	for (auto& player : Data.Players) {
		std::stringstream ss;
		ss << "Player " << playerNumber << " (" << player.second.Name << ")";
		std::string text = ss.str();
		ImGui::TextColored(ImVec4(0, 0, 0, 1), text.c_str());

		++playerNumber;
	}


	ImGui::SetCursorPosY(100);
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 600);
	ImGui::TextColored(ImVec4(0, 0, 0, 1), "Press O to switch element");
	ImGui::PopFont();
}
