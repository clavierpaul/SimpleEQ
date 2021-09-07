#include "PluginProcessor.h"
#include "PluginEditor.h"

SimpleEQAudioProcessor::SimpleEQAudioProcessor()
     : AudioProcessor(BusesProperties()
                       .withInput("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                       )
{
}

SimpleEQAudioProcessor::~SimpleEQAudioProcessor()
= default;

const juce::String SimpleEQAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool SimpleEQAudioProcessor::acceptsMidi() const
{
   return false;
}

bool SimpleEQAudioProcessor::producesMidi() const
{
   return false;
}

bool SimpleEQAudioProcessor::isMidiEffect() const
{
   return false;
}

double SimpleEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleEQAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String SimpleEQAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void SimpleEQAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

void SimpleEQAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void SimpleEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool SimpleEQAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void SimpleEQAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data,(because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for(auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for(int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        juce::ignoreUnused(channelData);
        // ..do something to the data...
    }
}

bool SimpleEQAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SimpleEQAudioProcessor::createEditor()
{
//    return new SimpleEQAudioProcessorEditor(*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

void SimpleEQAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void SimpleEQAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleEQAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleEQAudioProcessor::createParameterLayout() {
    
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("lowCutFreq",
                                                           "Low Cut Freq",
                                                           juce::NormalisableRange<float>(20.f, 20'000.f, 1.f),
                                                           20.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("highCutFreq",
                                                           "High Cut Freq",
                                                           juce::NormalisableRange<float>(20.f, 20'000.f, 1.f),
                                                           20'000.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("notchFreq",
                                                           "Notch Freq",
                                                           juce::NormalisableRange<float>(20.f, 20'000.f, 1.f),
                                                           750.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("notchGain",
                                                           "Notch Gain",
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.05f),
                                                           0.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("notchQ",
                                                           "Notch Q",
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.1f),
                                                           1.f));
    
    juce::StringArray slopes { "12 dB/oct", "24 penisdB/oct", "36 dB/oct", "48 dB/oct" };
    
    layout.add(std::make_unique<juce::AudioParameterChoice>("lowSlope", "Low Cut Slope", slopes, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("highSlope", "High Cut Slope", slopes, 0));
    
    return layout;
}
