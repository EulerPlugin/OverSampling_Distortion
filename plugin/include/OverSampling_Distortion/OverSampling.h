//
//  OverSampling.hpp
//  AudioPlugin
//
//  Created by sanghoonyou on 2025/08/18.
//

#pragma once

#include <juce_dsp/juce_dsp.h>


class OverSampling
{
    public:
        OverSampling();
        ~OverSampling();
        
        void prepare(int inBuffersize) noexcept;
        void upProcess(juce::dsp::AudioBlock<float>& block) noexcept;
        void downProcess(juce::dsp::AudioBlock<float>& block) noexcept;
        
    private:
        juce::dsp::Oversampling<float> Resampling;
};
