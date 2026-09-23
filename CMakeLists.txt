cmake_minimum_required(VERSION 3.22)

project(MixBuddy VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/JUCE)

juce_add_plugin(MixBuddy
        COMPANY_NAME "Mix Buddy"
        BUNDLE_ID "com.joework.mixbuddy"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE Mxbd
        PLUGIN_CODE Mbud
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "Mix Buddy"
        NEEDS_WEB_BROWSER FALSE
        NEEDS_CURL FALSE
)

target_sources(MixBuddy
        PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
)

target_include_directories(MixBuddy
        PRIVATE
        src
)

target_compile_definitions(MixBuddy
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(MixBuddy
        PRIVATE
        juce::juce_audio_utils
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)