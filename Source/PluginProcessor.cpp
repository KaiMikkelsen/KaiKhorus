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
                       ),
treeState(*this, nullptr, "PARAMETER", {std::make_unique<juce::AudioParameterFloat>("wetDry", "wetDry", 0.0f, 1.0f, 0.60f),//set 0.60f as default value as that's my favorite sound
                                        std::make_unique<juce::AudioParameterBool>("buttonOne", "buttonOne", false),
                                        std::make_unique<juce::AudioParameterBool>("buttonTwo", "buttonTwo", false)}
          )
#endif
{
    treeState.state = juce::ValueTree("savedParams");
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
    delayBufferSize = sampleRate * maxBufferDelay;
    delayBuffer.setSize(getTotalNumOutputChannels(), delayBufferSize);
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
    if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono())
    {
        // Mono-to-mono
        if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono())
            return true;
    }
    else if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
    {
        // Mono-to-stereo OR stereo-to-stereo
        if ((layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()) || (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()))
            return true;
    }

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
    float phase;
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        fillDelayBuffer(buffer, channel, 1.0f);
        phase = lfoPhase;
        float* channelData = buffer.getWritePointer (channel);


        auto *chorusOneButton = treeState.getRawParameterValue("buttonOne");
        auto *chorusTwoButton = treeState.getRawParameterValue("buttonTwo");
        auto wetDry = treeState.getRawParameterValue("wetDry");
        

        for(int sample = 0; sample < buffer.getNumSamples(); sample++)
        {

            float out = 0;
            float secondOut = 0;
            if(*chorusOneButton && !*chorusTwoButton)
            {
                width = button1Width;
                frequency = button1Frequency;
                out = calculateOut(channel, sample, phase, width, frequency);
            }
            else if(*chorusTwoButton && !*chorusOneButton)
            {
                width = button2Width;
                frequency = button2Frequency;
                out = calculateOut(channel, sample, phase, width, frequency);
            }
            else if(*chorusTwoButton && *chorusOneButton)
            {
                width = button1Width;
                frequency = button1Frequency;
                out = calculateOut(channel, sample, phase, width, frequency);
                
                width = button2Width;
                frequency = button2Frequency;
                secondOut = calculateOut(channel, sample, phase, width, frequency);
                
            }
            else
            {
                /*
                width = 0;
                frequency = 0;
                out = calculateOut(channel, sample, phase, width, frequency);
                 */
            }
            
            
            
            if(channel == 0)
            {//THE ONE WITH ISSUES
                channelData[sample] = channelData[sample] * (1.0f - *wetDry) +  *wetDry * out + *wetDry * secondOut;
                
            }
            else
            {
                channelData[sample] = channelData[sample] * (1.0f - *wetDry) + *wetDry * out + *wetDry * secondOut;
                //channelData[sample] = 0;
            
            }
            
            
           

            phase += frequency * (1.0f / (float) getSampleRate());
            if (phase >= 1.0f)
            {
                phase -= 1.0f;
            }

        }

    }
    updateBufferPositions(buffer, delayBuffer);
    lfoPhase = phase;
    
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}


float KaiKhorusAudioProcessor::calculateOut(int channel, int sample, float phase, float width, float frequency)
{
    float localDelayTime;
    if(channel == 0)
    {
        phase = phase + 0.5f;
        if (phase >= 1.0f)
        {
            phase -= 1.0f;
        }
        localDelayTime = width * lfo(phase) * (float)getSampleRate();//Delay Time in ms
    }
    else
    {
        localDelayTime = width * lfo(phase) * (float)getSampleRate();//Delay Time in ms
    }
    
  
    float delayWritePosition = delayBufferSize + writePosition + sample - localDelayTime;
    float readPosition = fmodf(delayWritePosition, delayBufferSize);
          
    int localReadPosition = floorf (readPosition);
   
    float out = cubicInterpolation(channel, readPosition, localReadPosition);
    
    return out;
    
    
    
}

float KaiKhorusAudioProcessor::cubicInterpolation(int channel, float readPosition, int localReadPosition)
{
    
    float* delayData = delayBuffer.getWritePointer (channel);
    
    float fraction = readPosition - (float)localReadPosition;
    float fractionSqrt = fraction * fraction;
    float fractionCube = fractionSqrt * fraction;

    float sample0 = delayData[(localReadPosition - 1 + delayBufferSize) % delayBufferSize];
    float sample1 = delayData[(localReadPosition + 0)];
    float sample2 = delayData[(localReadPosition + 1) % delayBufferSize];
    float sample3 = delayData[(localReadPosition + 2) % delayBufferSize];

    float a0 = - 0.5f * sample0 + 1.5f * sample1 - 1.5f * sample2 + 0.5f * sample3;
    float a1 = sample0 - 2.5f * sample1 + 2.0f * sample2 - 0.5f * sample3;
    float a2 = - 0.5f * sample0 + 0.5f * sample2;
    float a3 = sample1;
    return a0 * fractionCube + a1 * fractionSqrt + a2 * fraction + a3;
    
}

float KaiKhorusAudioProcessor::lfo(float phase)
{
    //float value = 0.5f + 0.5f * sinf (2.0f * M_PI * phase); for sin wave lfo
    float out;
    if (phase < 0.25f)
        out = 0.5f + 2.0f * phase;
    else if (phase < 0.75f)
        out = 1.0f - 2.0f * (phase - 0.25f);
    else
        out = 2.0f * (phase - 0.75f);
    
   
    return out; //osicllate between values of 0 and 1
    
    
}

void KaiKhorusAudioProcessor::updateBufferPositions(juce::AudioBuffer<float>&buffer, juce::AudioBuffer<float>&delayBuffer)
{
        auto bufferSize = buffer.getNumSamples();
    
        writePosition += bufferSize;
        writePosition %= delayBufferSize;
    
    
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
    std::unique_ptr<juce::XmlElement> xml (treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void KaiKhorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<juce::XmlElement> theParams (getXmlFromBinary(data, sizeInBytes));
    
    if(theParams != nullptr)
    {
        if(theParams  -> hasTagName(treeState.state.getType()))
        {
            treeState.state = juce::ValueTree::fromXml(*theParams);
            
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KaiKhorusAudioProcessor();
}

