#pragma once

#include "BlackBirdBox/Core/Application.h"
#include "BlackBirdBox/Core/Log.h"
#include "BlackBirdBox/Core/Macros.h"

#ifdef PLATFORM_WINDOWS

extern Core::Application* Core::CreateApplication();

int main(int argc, char** argv)
{
	Core::Log::Init();

	PROFILE_BEGIN_SESSION("Profile-Startup.json");
	auto app = Core::CreateApplication();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Profile-Runtime.json");
	app->Run();
	PROFILE_END_SESSION();

	PROFILE_BEGIN_SESSION("Profile-Shutdown.json");
	delete app;
	PROFILE_END_SESSION();
}

#endif
