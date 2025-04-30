#include <Skateboard.h>
#include "Skateboard/EntryPoint.h"
//#include "TestLayer.h"
//#include "GameLayer.h"
#include "MainLayer.h"
#include <cassert>
//#include "RaytracingLayer.h"
#include "Skateboard/Audio/AudioRenderer.h"
#include "Skateboard/Audio/AudioRenderCommand.h"
#include "Skateboard/Audio/AudioFile.h"
#include "pad.h"

#include "Skateboard/Audio/Platforms/Playstation/PlaystationAudioInstance.h"
class GameApp : public Skateboard::Application
{
public:
	GameApp()
	{
		// Initialise some layers
		//PushLayer(new TestLayer());
		//PushLayer(new GameLayer());
		//PushLayer(new RaytracingLayer());

		PushLayer(new MainLayer());
	}
};

Skateboard::Application* Skateboard::CreateApplication(int argc, char** argv)
{
	return new GameApp();
}