//
// Created by Sumi on 23/09/2026.
//

#pragma once

#include <juce_core/juce_core.h>



class Config
{
public:
    static constexpr bool MIXBUDDY_DEBUG = true;
    static constexpr const char* MIXBUDDY_LOG_DEBUG_DIRECTORY = "~/CLionProjects/mix-buddy/logs/mix-buddy.log";
    static constexpr const char* MIXBUDDY_LOG_DIRECTORY = "~/Library/MixBuddy/logs/mix-buddy.log";
};
