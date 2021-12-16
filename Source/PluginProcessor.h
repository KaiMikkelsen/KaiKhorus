/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class KaiKhorusAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    KaiKhorusAudioProcessor();
    ~KaiKhorusAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float width, frequency;
    float maxBufferDelay {20.0f};
    float wetDry;
    

private:
    
    //KaiKhorusAudioProcessorEditor& editor;
    
    juce::AudioBuffer<float> delayBuffer;
    void fillDelayBuffer(juce::AudioBuffer<float>& delayBuffer, int channel, float feedbackGain);
    int getDelayTime(int milliseconds);
    void readFromDelayBuffer(int channel, juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer);
    void updateBufferPositions(juce::AudioBuffer<float>&buffer, juce::AudioBuffer<float>&delayBuffer);
    float cubicInterpolation(int channel, float readPosition, int localReadPosition);
    float lfo(float phase);
    
    
    int delayBufferSize;
    int writePosition {0};
    int readPosition {0};
    double maxNumberofDelaySeconds {2};
    float lfoPhase {0};
    
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KaiKhorusAudioProcessor)
};
