#pragma once

#include "BlackBirdBox/Core/Core.h"
#include "BlackBirdBox/Core/Application.h"
#include "BlackBirdBox/Core/Log.h"

#ifdef PLATFORM_WINDOWS

extern Core::Application* Core::CreateApplication();

int main(int argc, char** argv)
{
	Core::Log::Init();

	//PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	auto app = Core::CreateApplication();
	//PROFILE_END_SESSION();

	//PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
	//PROFILE_END_SESSION();

	//PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Shutdown.json");
	delete app;
	//PROFILE_END_SESSION();
}

#endif
