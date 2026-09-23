//
// Created by Cosmo on 23/09/2026.
//
#pragma once

#include <juce_core/juce_core.h>

/**
 * @class Log
 * @brief Logging class for the plugin. Logs are output to files only.
 */
class Log
{
public:
    static void initialise();
    static void shutdown();

    static void info(const juce::String& message);
    static void error(const juce::String& message);

    static juce::File getLogFile();
};
