/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KaiKhorusAudioProcessor::KaiKhorusAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

KaiKhorusAudioProcessor::~KaiKhorusAudioProcessor()
{
}

//==============================================================================
const juce::String KaiKhorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KaiKhorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KaiKhorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KaiKhorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KaiKhorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KaiKhorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KaiKhorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KaiKhorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KaiKhorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void KaiKhorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KaiKhorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto delayBufferSize = sampleRate * maxNumberofDelaySeconds;
    delayBuffer.setSize(getTotalNumOutputChannels(), (int)delayBufferSize);
    
    width = 1.0f;
    frequency = 1.0f;
    
    
}

void KaiKhorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KaiKhorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KaiKhorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float phase = lfoPhase;
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        fillDelayBuffer(buffer, channel, 1.0f);
        for(int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            
            
            
            
            float localDelayTime = width * lfo(phase); //* (float)getSampleRate();
            
            DBG("delaytimeis is: " << localDelayTime);
            
            
            
            phase += frequency * (1.0f / (float) getSampleRate());
            if (phase >= 1.0f)
            {
                phase -= 1.0f;
            }
            
            
        }
        
        
        //auto* channelData = buffer.getWritePointer (channel);
        
        //readFromDelayBuffer(channel, buffer, delayBuffer);
        //fillDelayBuffer(buffer, channel, feedback);
     
    }
    lfoPhase = phase;
    updateBufferPositions(buffer, delayBuffer);
 
    
    
}


float KaiKhorusAudioProcessor::lfo(float phase)
{
    return (0.5f + 0.5f * sinf (2.0f * M_PI * phase)); //osicllate between values of 0 and 1
    
    
}

void KaiKhorusAudioProcessor::updateBufferPositions(juce::AudioBuffer<float>&buffer, juce::AudioBuffer<float>&delayBuffer)
{
        auto bufferSize = buffer.getNumSamples();
        auto delayBufferSize = delayBuffer.getNumSamples();
    
        writePosition += bufferSize;
        writePosition %= delayBufferSize;
    
    
}

void  KaiKhorusAudioProcessor::readFromDelayBuffer(int channel, juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int readPosition)
{
    //1 second of audio from the past
    //getsamplerate = 1 second
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
//    int readPosition = static_cast<int>(delayBufferSize + writePosition - (getSampleRate() * 1 / 1000)) % delayBufferSize;
    float startGain = 1.0f;
    float endGain = startGain;
    
    if(readPosition < 0)
    {
        readPosition += delayBufferSize;
    }
    
    if(readPosition + bufferSize < delayBufferSize)
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, startGain, endGain);
    }
    else
    {
        auto numSamplesToEnd = delayBufferSize - readPosition;
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesToEnd, startGain, endGain);
        
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        buffer.addFromWithRamp(channel, numSamplesToEnd, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, startGain, endGain);
        
        
    }

    
}


void KaiKhorusAudioProcessor::fillDelayBuffer(juce::AudioBuffer<float>& buffer, int channel, float feedbackGain)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    float startGain = feedbackGain;
    float endGain = feedbackGain;
    
    if(delayBufferSize > bufferSize + writePosition)
    {
        
        delayBuffer.copyFromWithRamp(channel, writePosition, buffer.getReadPointer (channel), bufferSize, startGain, endGain);
        
    }
    else
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        
        auto numSamplesToEnd = delayBufferSize - writePosition;
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        
        //Copy all the way to end
        delayBuffer.copyFromWithRamp(channel, writePosition, buffer.getWritePointer (channel), numSamplesToEnd, startGain, endGain);
        
        //Now copy the rest to the beginning
        delayBuffer.copyFromWithRamp(channel, 0, channelData + numSamplesToEnd, numSamplesAtStart, startGain, endGain);
        
   
    }
   
    
    
}


//==============================================================================
bool KaiKhorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* KaiKhorusAudioProcessor::createEditor()
{
    return new KaiKhorusAudioProcessorEditor (*this);
}

//==============================================================================
void KaiKhorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KaiKhorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KaiKhorusAudioProcessor();
}


/*
void  KaiKhorusAudioProcessor::readFromDelayBuffer(int channel, juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int readPosition)
{
    //1 second of audio from the past
    //getsamplerate = 1 second
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();
    
//    int readPosition = static_cast<int>(delayBufferSize + writePosition - (getSampleRate() * 1 / 1000)) % delayBufferSize;
    float startGain = 1.0f;
    float endGain = startGain;
    
    if(readPosition < 0)
    {
        readPosition += delayBufferSize;
    }
    
    if(readPosition + bufferSize < delayBufferSize)
    {
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), bufferSize, startGain, endGain);
    }
    else
    {
        auto numSamplesToEnd = delayBufferSize - readPosition;
        buffer.addFromWithRamp(channel, 0, delayBuffer.getReadPointer(channel, readPosition), numSamplesToEnd, startGain, endGain);
        
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        buffer.addFromWithRamp(channel, numSamplesToEnd, delayBuffer.getReadPointer(channel, 0), numSamplesAtStart, startGain, endGain);
        
        
    }

    
}
*/
