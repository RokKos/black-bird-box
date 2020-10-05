#include "bbbpch.h"

#include "AudioContext.h"

namespace BlackBirdBox {

SoLoud::Soloud AudioContext::so_loud_;

void AudioContext::Init()
{
    PROFILE_FUNCTION();

    LOG_INFO("Initializg SoLoud Audio Engine");
    so_loud_.init();
}

void AudioContext::DeInit()
{
    PROFILE_FUNCTION();

    LOG_INFO("De-Initializg SoLoud Audio Engine");
    so_loud_.deinit();
}

void AudioContext::PlaySample(const Ref<AudioSample>& sample)
{
    PROFILE_FUNCTION();

    LOG_TRACE("Playing Audio Sample: {0} with Volume {1}, lenght {2} and Loopping: {3}", sample->GetName(), sample->GetVolume(),
        sample->GetLenghtInSec(), sample->GetLooping() ? "On" : "Off");

    so_loud_.play(sample->GetWawSource());
}
}
