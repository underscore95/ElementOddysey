#pragma once

#include "Skateboard\Scene\Components.h"


class SpriteAnimationComponent :
	public Skateboard::BaseComponent
{
public:

	friend class SpriteAnimationSystem;

	void Play(int track, bool loop = true, bool reverse = false) {
		NewTrack = track;
		Restart = true;
		Playing = true;
		Loop = loop;
	}

	//controls, like push buttons on a cassete tape player
	bool Playing = false;
	bool Paused = false;
	bool Loop = false;
	bool Restart = false;

	//controls the flow of the FrameIncremen, multiplying it by -1 in case of renerse so that the animation plays backwards
	bool Reverse = false;

	//mirors the animation in the respective axis, NOTE: this doesnt affect the animation playback direction, we will start walking backwards, for that see reverse :3
	bool FlipX = false;
	bool FlipY = false;

	//bool switched_track
	int NewTrack = -1;

	//what is the curent frame we displaying
	float ElapsedTime = 0;
	int32_t CurrentFrame = 0;

	//how fast we are moving FPS
	float FrameRate;

	struct Track
	{
		//size of the frame expressed in Pixels
		int2 FrameSize;

		//where in the spritesheet are we starting from in Pixels
		int2 StartOffset = glm::vec2(0, 0);

		//frame count in a track
		int32_t FrameCount;

		//where are we going for the next frame, expressed as direction vector  so that on increment newFrame = StartOffset + FrameSize * Frameincrement * ++currentframe
		float2 FrameIncrement;
	};

	std::vector<Track> Tracks;

private:
	float2 UvOffset = { 0,0 };
	int CurrentTrack = -1;
};

typedef SpriteAnimationComponent SpriteAnimComponent;