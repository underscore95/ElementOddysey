#include "MusicManager.h"

MusicManager::MusicManager() : category { MENU_MUSIC }
{
	music.resize(NUM_MUSIC_CATEGORIES);

	RegisterMusic();

	assert(!music[MENU_MUSIC].empty());
	currentTrack = music[MENU_MUSIC][rand() % music[MENU_MUSIC].size()]; // Pick a random menu track to start with
	Skateboard::AudioRenderer::PlayMusic(currentTrack, true);
}

void MusicManager::AddTrack(const MusicCategory category, const std::wstring& filePath)
{
	const std::string name = std::to_string(numMusicTracks);
	Skateboard::AssetManager::LoadAudio(filePath, name);
	music[category].push_back(name);

	++numMusicTracks;
}

MusicManager& MusicManager::Singleton()
{
	static MusicManager musicManager;
	return musicManager;
}

void MusicManager::SetMusic(const MusicCategory category)
{
	// Fade this music out and fade in next music
	this->decreasingVolume = true;
	if (category == this->category && music[category].size() > 1) {
		// Prevent playing the same track again
		const auto last = currentTrack;
		do {
			nextTrack = music[category][rand() % music[category].size()];
		} while (last == nextTrack);
	}
	else {
		nextTrack = music[category][rand() % music[category].size()];
	}

	nextTrackExists = true;
	this->category = category;
}

void MusicManager::Update(Skateboard::TimeManager* time)
{
	// Handle volume decreasing
	if (decreasingVolume && volume > MIN_VOLUME) {
		volume -= time->DeltaTime() * VOLUME_DECREASE_PER_SECOND;
		if (volume < MIN_VOLUME) {
			volume = MIN_VOLUME;
			decreasingVolume = false;

			// We reached min volume, transition to next track
			Skateboard::AudioRenderer::StopAudio(currentTrack);

			currentTrack = nextTrack;
			nextTrackExists = false;
			Skateboard::AudioRenderer::PlayMusic(nextTrack, true); // TODO: Detect music ending and pick a new one (hard since we need the fade out... or maybe we don't?)
		}
		else {
			Skateboard::AudioRenderer::SetAudioVolume(currentTrack, volume);
		}
	}

	// Handle volume increasing
	if (volume < MAX_VOLUME && !decreasingVolume) {
		volume += time->DeltaTime() * VOLUME_INCREASE_PER_SECOND;
		if (volume > MAX_VOLUME) {
			volume = MAX_VOLUME;
		}
		Skateboard::AudioRenderer::SetAudioVolume(currentTrack, volume);
	}
}
