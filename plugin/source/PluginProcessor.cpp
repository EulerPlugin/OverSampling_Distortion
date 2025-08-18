#include "OverSampling_Distortion/PluginProcessor.h"
#include "OverSampling_Distortion/PluginEditor.h"

namespace OverSampling_Distortion {

OverSampling_DistortionAudioProcessor::OverSampling_DistortionAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ) {
}

OverSampling_DistortionAudioProcessor::~OverSampling_DistortionAudioProcessor() {}

const juce::String OverSampling_DistortionAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool OverSampling_DistortionAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool OverSampling_DistortionAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool OverSampling_DistortionAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double OverSampling_DistortionAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int OverSampling_DistortionAudioProcessor::getNumPrograms() {
  return 1;
}

int OverSampling_DistortionAudioProcessor::getCurrentProgram() {
  return 0;
}

void OverSampling_DistortionAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String OverSampling_DistortionAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void OverSampling_DistortionAudioProcessor::changeProgramName(int index,
                                                              const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void OverSampling_DistortionAudioProcessor::prepareToPlay(double sampleRate,
                                                          int samplesPerBlock)
{
  juce::ignoreUnused(sampleRate, samplesPerBlock);
  Resampling.prepare(samplesPerBlock);
  
}

void OverSampling_DistortionAudioProcessor::releaseResources() {
}

bool OverSampling_DistortionAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void OverSampling_DistortionAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                         juce::MidiBuffer& midiMessages)
{
      juce::ignoreUnused(midiMessages);

      juce::ScopedNoDenormals noDenormals;
      auto totalNumInputChannels = getTotalNumInputChannels();
      auto totalNumOutputChannels = getTotalNumOutputChannels();

      for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
      
      juce::dsp::AudioBlock<float> block (buffer);

      Resampling.upProcess(block);
      
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
      {
            for (int i = 0; i < static_cast<int>(block.getNumSamples()); ++i)
            {
                auto* channeldata = block.getChannelPointer(static_cast<size_t>(channel));
                channeldata[i] =  mDistortion.process(channeldata[i]);
            }
      }
      
      Resampling.downProcess(block);
      
      
//      for (int channel = 0; channel < totalNumInputChannels; ++channel)
//      {
//        auto* channelData = buffer.getWritePointer(channel);
//        juce::ignoreUnused(channelData);
//
//        auto* inPutData = buffer.getReadPointer(channel);
//
//        for (int i = 0; i < buffer.getNumSamples(); ++i)
//        {
//            channelData[i] = mDistortion.process(inPutData[i]);
//        }
//      }
}

bool OverSampling_DistortionAudioProcessor::hasEditor() const {
  return true;
}

juce::AudioProcessorEditor* OverSampling_DistortionAudioProcessor::createEditor() {
  return new OverSampling_DistortionAudioProcessorEditor(*this);
}

void OverSampling_DistortionAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData) {
  juce::ignoreUnused(destData);
}

void OverSampling_DistortionAudioProcessor::setStateInformation(const void* data,
                                                                int sizeInBytes) {
  juce::ignoreUnused(data, sizeInBytes);
}

}  // namespace OverSampling_Distortion

// 글로벌 네임스페이스에 남겨둬야 함
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new OverSampling_Distortion::OverSampling_DistortionAudioProcessor();
}
