#pragma once
#include "soloud_wav.h"

namespace BlackBirdBox {

class AudioSample {
public:
    AudioSample(const std::string& audio_asset_file_path, float volume = 1.0f, bool looping = false);
    virtual ~AudioSample() = default;

    const SoLoud::Wav& GetWawSource() const;
    SoLoud::Wav& GetWawSource();
    const std::string& GetName() const;
    float GetVolume() const;
    bool GetLooping() const;
    double GetLenghtInSec() const;

    void SetVolume(float volume);
    void SetLooping(bool looping);

    void Stop();

private:
    SoLoud::Wav sample_;

    std::string name_;
    bool looping_ = false;
    float volume_ = 1.0f;
};
}
