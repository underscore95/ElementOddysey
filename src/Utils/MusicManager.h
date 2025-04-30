#pragma once

#include <Skateboard.h>

typedef char MusicCategory;
inline const static MusicCategory MENU_MUSIC = 0;
inline const static MusicCategory GAME_MUSIC = 1;
inline const static MusicCategory LOSE_MUSIC = 2;
inline const static MusicCategory FIRST_MUSIC_CATEGORY = MENU_MUSIC;
inline const static MusicCategory NUM_MUSIC_CATEGORIES = 3;

class MusicManager
{
private:
	static constexpr float VOLUME_FADE_IN_TIME = 1; // in seconds
	static constexpr float VOLUME_FADE_OUT_TIME = 2; // in seconds
	static constexpr float VOLUME_INCREASE_PER_SECOND = 1.0f / VOLUME_FADE_IN_TIME;
	static constexpr float VOLUME_DECREASE_PER_SECOND = 1.0f / VOLUME_FADE_OUT_TIME;
	static constexpr float MIN_VOLUME = 0;
	static constexpr float MAX_VOLUME = 0.5; // Will be this volume most of the time, except during fades.

	void RegisterMusic() {
		// ---------------------------
		// --- REGISTER MUSIC HERE ---
		// ---------------------------

		AddTrack(MENU_MUSIC, L"/app0/assets/audio/menu_music/1.wav");
		AddTrack(MENU_MUSIC, L"/app0/assets/audio/menu_music/2.wav");

		AddTrack(GAME_MUSIC, L"/app0/assets/audio/game_music/1.wav");
		AddTrack(GAME_MUSIC, L"/app0/assets/audio/game_music/2.wav");

		AddTrack(LOSE_MUSIC, L"/app0/assets/audio/lose_music/1.wav");

		// ----------------------------------
		// --- END REGISTERING MUSIC HERE ---
		// ----------------------------------
	}

	int numMusicTracks{ 0 };
	float volume{ MIN_VOLUME };
	bool decreasingVolume{ false };
	MusicCategory category{ MENU_MUSIC };
	std::string currentTrack;
	std::string nextTrack;
	bool nextTrackExists{ false };
	std::vector<std::vector<std::string>> music;

	MusicManager();
	void AddTrack(const MusicCategory category, const std::wstring& filePath);
public:
	static MusicManager& Singleton();
	
	void SetMusic(const MusicCategory category);

	void Update(Skateboard::TimeManager* time);
};

