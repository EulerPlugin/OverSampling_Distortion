# Oversampling From Distortion

### 1)Why is oversampling needed? 
Nonlinear distortion (e.g., `tanh`) generate harmonics. Some of there exceed the Nyquist frequency (Fs/2) of the original sample rate. If you keep the original rate or downsample directly, thos out-of-band components fold back(alias) into the spectrum, producing spurious peaks.

<br>

### Solutionn Principle : 

Perform the nonlinear processing at a higher sample rate (where there's enough headroom), then remove the unnecessary high-frequency content, and finally bring the signal back down to the original sample rate.

~~~cpp
Input Signal 
     │
     ▼
[ Oversampling ]
     │
     ▼
[ Nonlinear Distortion ]
     │
     ▼
[ Downsampling ]
     │
     ▼
Output Signal
~~~

<br>

### Upsampling Implementation
"Zero insertion + interpolation(low-pass) filter"
The Interpolation filter's cutoff should be placed at the Nyquist frequency of the input signal 

<img width="850" height="292" alt="image" src="https://github.com/user-attachments/assets/c5ec8cc7-c182-4bf0-a079-0e08e31941e0" />


<br>


The graphs below illustrate the interpolation caused by the low-pass filter. Linear interpolation uses two neighboring samples to compute all in-between samples.

<img width="833" height="383" alt="image" src="https://github.com/user-attachments/assets/03484b30-2ee8-496f-b933-6db4ebf4cd78" />


<img width="833" height="383" alt="image" src="https://github.com/user-attachments/assets/b19f3838-2823-4a17-a649-06d61f6dec52" />




<br>
<br>
<br>


### Downsampling Implementation
Pre-aliasing(low-pass) filter first, and then discatd the samples. The cutoff frequency of the low-pass filter is the half of the target sample rate. 


<img width="752" height="264" alt="image" src="https://github.com/user-attachments/assets/7b8d1dfa-d8b9-44b1-bae3-57e4db8762d9" />


As shown in the below graph, if we downsample without passing through a pre-aliasing fiilter, the Nyquist frequency becomes lower, which can cause aliasing


<img width="757" height="456" alt="image" src="https://github.com/user-attachments/assets/caae6d2b-2373-470a-8270-1f1d5aa4a91f" />


<br>
<br>


### 2) Implementation Overview
This project uses `juce::dsp::Oversampling<floa>` to handle oversampling JUCE's Oversampling class provides:

- Block upsampling/downsampling
- Built-in interpolation (anti-imaging) and pre-aliasing filters
- Choice between FIR and IIR implementations
- Automatic delay compensation

The main purpose is to inser oversampling stages **before and after the nonlonear distortion block** to suppress aliasing artifacts.

<br>

### Core Processing

~~~cpp
juce::dsp::AudioBlock<float> block (buffer);

// Upsample
juce::dsp::AudioBlock<float> upsampleBlock = Resampling.upProcess(block);

// Apply distortion in oversampled domain
for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
{
    auto* channelData = upsampleBlock.getChannelPointer(static_cast<size_t>(channel));
    
    for (int i = 0; i < static_cast<int>(upsampleBlock.getNumSamples()); ++i)
    {
        channelData[i] = mDistortion.process(channelData[i]);
    }
}

// Downsample (with pre-aliasing filter)
Resampling.downProcess(block);
~~~

Supporting functions:
~~~cpp
void OverSampling::prepare(int bufferSize) noexcept
{
    Resampling.initProcessing(static_cast<size_t>(bufferSize));
}

juce::dsp::AudioBlock<float> OverSampling::upProcess(juce::dsp::AudioBlock<float>& block) noexcept
{
    return Resampling.processSamplesUp(block);
}

void OverSampling::downProcess(juce::dsp::AudioBlock<float>& block) noexcept
{
    Resampling.processSamplesDown(block);
}

float DistortionDSP::process(const float inSample) noexcept
{
    return std::tanh(G * inSample) / std::tanh(G);
}
~~~

<br>
<br>

### Key Considerations
`juce::dsp::AudioBlock<T>` is a **lightweight wrapper around raw audio buffers.** It provides a higher-level interface for accessing and processing audio data, commomly used in JUCE's DSP module.

Instead of manually handing raw `floa*` or `juce::AudioBuffer<float>`, `AudioBlock` offers convenient functions to:

- Access per-channel pointers
- Iterate over samples
- Slice audio into sub-blocks
- Interperate easily JUCE DSP classes

When an AudioBlock is constructed, it already creates a view of the given buffer by internally holding a pointer to its data. In other words, it doesn’t make a deep copy — it just references the existing buffer’s memory.



<br>
<br>


### About Polyphase Filters
JUCE’s oversampling can use **polyphase FIR filters.**
- A polyphase filter splits a large FIR filter into several sub-filters (phases).
- During upsampling, instead of convolving with a long filter (mostly zeros due to zero insertion), the polyphase structure reuses only the non-zero branches. This reduces redundant computations.
- Similarly, in downsampling, polyphase filtering avoids computing samples that will be discarded.
- Benefit: Same frequency response as the full FIR design but with much lower CPU cost, which is crucial for real-time audio DSP.


<img width="876" height="457" alt="image" src="https://github.com/user-attachments/assets/d24f39fd-430d-4681-8d6a-066cd1ab5afd" />

<br>
<br>


### 3) Measurement

<img width="1722" height="914" alt="image" src="https://github.com/user-attachments/assets/ccd71fab-91c7-49ab-99a5-b1680bb2fe6a" />

Result of **Oversample → Distortion → Downsample.**
- Fundamental frequency plus natural harmonic series.
- Smooth roll-off near the band edge (filter effect).
- Almost no aliasing spikes → working as intended.


<br>


<img width="1722" height="914" alt="image" src="https://github.com/user-attachments/assets/a09aff9e-65a3-4db7-a463-ee6c558e4de9" />

Case when oversampling is skipped or filtering is insufficient.
- Harmonics above Nyquist fold back (alias) and appear as spurious spikes at incorrect positions.

<br>

<img width="1722" height="914" alt="image" src="https://github.com/user-attachments/assets/51de3cbe-21e8-4cb4-a5f3-60e6650ace99" />

Proper harmonics when oversampling/filtering works correctly.
- Harmonics decay in a regular pattern.
- No abnormal peaks in the low–mid frequency range.


<br>

<img width="1722" height="914" alt="image" src="https://github.com/user-attachments/assets/27d000f4-d806-4b24-85ba-1887c5cbc717" />

Pure sine wave (before distortion)
- Only a single spectral peak exists.

