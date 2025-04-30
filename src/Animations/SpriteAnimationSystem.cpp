#include "SpriteAnimationSystem.h"
#include "Skateboard/Scene/ECS.h"
#include "SpriteAnimationComponent.h"
#include "Skateboard/Renderer/SpriteComponent.h"
#include "Skateboard/Platform.h"
#include "GameScene.h"

#define SKTBD_LOG_COMPONENT "SPRITE ANIMATOR SYSTEM"
#include "Skateboard/Log.h" 

void SpriteAnimationSystem::Init(GameScene* scn)
{
}


void SpriteAnimationSystem::HandleInput(Skateboard::TimeManager* time, GameScene* scn)
{
}

void SpriteAnimationSystem::RunUpdate(Skateboard::TimeManager* time, GameScene* scn)
{
	auto AnimatedSprites = Skateboard::ECS::GetRegistry().view<SpriteAnimationComponent, Skateboard::SpriteComponent>();

	AnimatedSprites.each([time](const auto entity, SpriteAnimationComponent& animateur, Skateboard::SpriteComponent& animatee)
		{
			//if the track changed updated the members of the animator
			if (animateur.NewTrack != animateur.CurrentTrack)
			{
				animateur.CurrentTrack = animateur.NewTrack;

				float2 t_size;
				t_size.x = animatee.GetRenderTexture()->GetWidth();
				t_size.y = animatee.GetRenderTexture()->GetHeight();

				float2 framesize = animateur.Tracks[animateur.CurrentTrack].FrameSize;
				float2 offset = animateur.Tracks[animateur.CurrentTrack].StartOffset;

				animateur.UvOffset = offset / t_size;
				float2 newScale = framesize / t_size;

				if (animatee.GetUvScale() != newScale)
					animatee.SetUvScale(newScale);

				animatee.SetUvOffset(animateur.UvOffset);

				animateur.ElapsedTime = 0;
			}

			//if we want to start from the beginning
			if (animateur.Restart)
			{
				animateur.CurrentFrame = 0;
				animateur.Restart = false;
			}

			//if we want to mirror the frame X;
			if (animateur.FlipX)
			{
				animatee.SetUvScale({ -animatee.GetUvScale().x, animatee.GetUvScale().y });
				animateur.FlipX = false;
			}

			//mirror Y
			if (animateur.FlipY)
			{
				animatee.SetUvScale({ animatee.GetUvScale().x, -animatee.GetUvScale().y });
				animateur.FlipY = false;
			}

			//if playing the animation, when we pass the elapsed time we want to update the animation offset in the sprite sheet
			if (animateur.Playing && !animateur.Paused)
			{
				animateur.ElapsedTime += time->DeltaTime();

				SpriteAnimationComponent::Track track = animateur.Tracks[animateur.CurrentTrack];

				if (animateur.ElapsedTime > 1.f / animateur.FrameRate)
				{
					animateur.ElapsedTime = 0;
					++animateur.CurrentFrame;

					if (animateur.CurrentFrame < track.FrameCount)
					{
						//SKTBD_APP_TRACE("animation frame: {}", animateur.CurrentFrame);
						animatee.SetUvOffset(animateur.UvOffset + animatee.GetUvScale() * animateur.CurrentFrame * track.FrameIncrement * ((animateur.Reverse) ? -1 : 1));
					}
					else if (animateur.Loop)
					{
						animateur.Restart = true;
					}
					else
					{
						animateur.Playing = false;
					}
				}
			}
		}
	);
}