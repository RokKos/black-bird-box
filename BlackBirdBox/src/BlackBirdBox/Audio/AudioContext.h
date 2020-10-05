#pragma once

#include "AudioSample.h"
#include "BlackBirdBox/Core/Core.h"
#include "soloud.h"

namespace BlackBirdBox {

class AudioContext {
public:
    static void Init();
    static void DeInit();

    static void PlaySample(const Ref<AudioSample>& sample);

private:
    static SoLoud::Soloud so_loud_; // SoLoud engine core
};
}
