//
//  OverSampling.cpp
//  AudioPlugin
//
//  Created by sanghoonyou on 2025/08/18.
//


#include <OverSampling_Distortion/OverSampling.h>


OverSampling::OverSampling()
:Resampling(2, 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR)
{
    
}

OverSampling::~OverSampling()
{

}
        
void OverSampling::prepare(int inBuffersize) noexcept
{
    Resampling.initProcessing(static_cast<size_t>(inBuffersize));
}

juce::dsp::AudioBlock<float> OverSampling::upProcess(juce::dsp::AudioBlock<float>& block) noexcept
{
     return Resampling.processSamplesUp(block);
}

void OverSampling::downProcess(juce::dsp::AudioBlock<float>& block) noexcept
{
     Resampling.processSamplesDown(block);
}
