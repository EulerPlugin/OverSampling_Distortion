//
//  DistortionDSP.hpp
//  AudioPlugin
//
//  Created by sanghoonyou on 2025/08/18.
//

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <stdio.h>

class DistortionDSP
{
    public:
        DistortionDSP();
        ~DistortionDSP();
        
        void prepare() noexcept;
        float process(const float inPutSample) noexcept;
        
    private:
        const float G = 10.0f;
        
};

