#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(18, 20, 22));

    auto bounds = getLocalBounds().reduced(24);

    g.setColour(juce::Colour::fromRGB(235, 238, 242));
    g.setFont(juce::FontOptions(28.0f, juce::Font::bold));
    g.drawFittedText("Mix Buddy - ALEMBIC", bounds.removeFromTop(48), juce::Justification::centred, 1);

    g.setColour(juce::Colour::fromRGB(150, 158, 168));
    g.setFont(juce::FontOptions(15.0f));
    g.drawFittedText("Track analyis & mix guidence", bounds.removeFromTop(28), juce::Justification::centred, 1);

    g.setColour(juce::Colour::fromRGB(82, 91, 102));
    g.drawRoundedRectangle(bounds.toFloat().reduced(20.0f), 8.0f, 1.0f);

    g.setColour(juce::Colour::fromRGB(170, 178, 188));
    g.setFont(juce::FontOptions(14.0f));
    g.drawFittedText("Audio pass through active", bounds.reduced(28), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
