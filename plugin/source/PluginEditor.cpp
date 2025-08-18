#include "OverSampling_Distortion/PluginEditor.h"
#include "OverSampling_Distortion/PluginProcessor.h"

namespace OverSampling_Distortion {

OverSampling_DistortionAudioProcessorEditor::OverSampling_DistortionAudioProcessorEditor(
    OverSampling_DistortionAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  juce::ignoreUnused(processorRef);
  setSize(400, 300);
}

OverSampling_DistortionAudioProcessorEditor::~OverSampling_DistortionAudioProcessorEditor() {}

void OverSampling_DistortionAudioProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
  g.drawFittedText("22222", getLocalBounds(),
                   juce::Justification::centred, 1);
}

void OverSampling_DistortionAudioProcessorEditor::resized() {
}

}  // namespace OverSampling_Distortion

