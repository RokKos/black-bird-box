#pragma once

#include "BlackBirdBox/Core/Application.h"
#include "BlackBirdBox/Core/Log.h"
#include "BlackBirdBox/Core/Macros.h"

#ifdef PLATFORM_WINDOWS

extern BlackBirdBox::Application* BlackBirdBox::CreateApplication();

int main(int argc, char** argv)
{
    BlackBirdBox::Log::Init();

    PROFILE_BEGIN_SESSION("Profile-Startup.json");
    auto app = BlackBirdBox::CreateApplication();
    PROFILE_END_SESSION();

    PROFILE_BEGIN_SESSION("Profile-Runtime.json");
    app->Run();
    PROFILE_END_SESSION();

    PROFILE_BEGIN_SESSION("Profile-Shutdown.json");
    delete app;
    PROFILE_END_SESSION();
}

#endif
