#include "bbbpch.h"

#include "AudioSample.h"

namespace BlackBirdBox {
AudioSample::AudioSample(const std::string& audio_asset_file_path, float volume, bool looping)
    : name_(audio_asset_file_path)
    , volume_(volume)
    , looping_(looping)
{
    PROFILE_FUNCTION();

    const SoLoud::result result = sample_.load(audio_asset_file_path.c_str());
    if (result == 0) {
        LOG_INFO("Audio Sample: %s succesfully loaded", audio_asset_file_path.c_str());
    } else {
        CORE_ASSERT(false, "Audio Sample: %s Unsuccesfully loaded", audio_asset_file_path.c_str());
    }

    sample_.setLooping(looping);
    sample_.setVolume(volume);
}

const SoLoud::Wav& AudioSample::GetWawSource() const { return sample_; }

SoLoud::Wav& AudioSample::GetWawSource() { return sample_; }

const std::string& AudioSample::GetName() const { return name_; }

float AudioSample::GetVolume() const { return volume_; }

bool AudioSample::GetLooping() const { return looping_; }

double AudioSample::GetLenghtInSec() const { return sample_.getLength(); }

void AudioSample::SetVolume(float volume)
{
    PROFILE_FUNCTION();

    sample_.setVolume(volume);
}

void AudioSample::SetLooping(bool looping)
{
    PROFILE_FUNCTION();

    sample_.setLooping(looping);
}

void AudioSample::Stop()
{
    PROFILE_FUNCTION();

    sample_.stop();
}
}
