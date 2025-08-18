//
//  DistortionDSP.cpp
//  AudioPlugin
//
//  Created by sanghoonyou on 2025/08/18.
//

#include <OverSampling_Distortion/DistortionDSP.h>
#include <cmath>

DistortionDSP::DistortionDSP()
{
    
}

DistortionDSP::~DistortionDSP()
{
    
}

void DistortionDSP::prepare() noexcept
{
    
}

float DistortionDSP::process(const float inPutSample) noexcept
{
    float outPutSample;
    
    outPutSample = tanh(G * inPutSample) / tanh(G);
    
    return outPutSample;
}
