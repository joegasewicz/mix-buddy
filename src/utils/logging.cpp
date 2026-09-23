//
// Created by Cosmo on 23/09/2026.
//

#include "logging.h"

#include <memory>

#include "config.h"


namespace
{
    std::unique_ptr<juce::FileLogger> logger;
    juce::CriticalSection loggerLock;

    // Namespaced helper to return a juce::File object with declared file path.
    juce::File getLogDirectory()
    {
        return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
            .getChildFile("MixBuddy")
            .getChildFile("logs");
    }
}

void Log::initialise()
{
    const juce::ScopedLock lock(loggerLock);

    if (logger != nullptr)
        return;

    juce::File logDir = getLogDirectory();
    logDir.getParentDirectory().createDirectory();


    logger = std::make_unique<juce::FileLogger>(
        getLogFile(),
        "Mix Buddy log started",
        1024 * 1024
    );
}

/**
 * @brief Lock the logger (juce::FileLogger)for the current scope
 */
void Log::shutdown()
{
    const juce::ScopedLock lock(loggerLock);
    logger.reset();
}

void Log::info(const juce::String &message)
{
    const juce::ScopedLock lock(loggerLock);

    if (logger == nullptr)
        initialise();
    if (logger != nullptr)
        logger->logMessage("[INFO] " + message);
}

void Log::error(const juce::String &message)
{
    const juce::ScopedLock lock(loggerLock);

    if (logger == nullptr)
        initialise();
    if (logger != nullptr)
        logger->logMessage("[ERROR] " + message);
}

/**
 * @brief logs are stored to:
 * $HOME/Library/MixBuddy/logs/mix-buddy.log
 * @return
 */
juce::File Log::getLogFile()
{
    if constexpr (Config::MIXBUDDY_DEBUG)
       return juce::File(Config::MIXBUDDY_LOG_DEBUG_DIRECTORY);

    return juce::File(Config::MIXBUDDY_LOG_DIRECTORY);
}