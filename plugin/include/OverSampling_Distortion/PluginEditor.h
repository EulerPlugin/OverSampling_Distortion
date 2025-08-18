#pragma once

#include "PluginProcessor.h"

namespace OverSampling_Distortion {

class OverSampling_DistortionAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit OverSampling_DistortionAudioProcessorEditor(OverSampling_DistortionAudioProcessor&);
  ~OverSampling_DistortionAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  OverSampling_DistortionAudioProcessor& processorRef;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverSampling_DistortionAudioProcessorEditor)
};

}
