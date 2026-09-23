# Agent Working Notes

## Repository Policy

Only the project owner should commit or submit repository changes.

Agents may directly update this `AGENTS.md` file. For other files, follow the user's current instruction and preserve any unrelated user changes.

## Current Project State

Mix Buddy is a C++20 JUCE audio plugin project built with CMake in CLion.

Current baseline:

- `external/JUCE` is present as a JUCE submodule/checkout.
- `CMakeLists.txt` defines a `MixBuddy` JUCE plugin target.
- Plugin formats include `AU`, `VST3`, and `Standalone`.
- The AU build has loaded successfully on a Logic Pro track.
- Source files currently live under `src/`:
  - `PluginProcessor.h`
  - `PluginProcessor.cpp`
  - `PluginEditor.h`
  - `PluginEditor.cpp`

Useful local commands:

```sh
cmake -S . -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --target MixBuddy_Standalone
cmake --build cmake-build-debug --target MixBuddy_AU
```

The built AU component is expected at:

```text
cmake-build-debug/MixBuddy_artefacts/Debug/AU/Mix Buddy.component
```

## Product Direction

Build a Logic Pro audio plugin that helps evaluate and eventually adjust mixes using AI.

The product should begin as a single-track analysis tool, then add structured parameter suggestions, then add a supervisor plugin for whole-mix coordination.

Keep audio pass-through reliable while building features incrementally.

## MVP 1: Single-Track AI Opinion

Goal: one plugin instance analyses one track and displays an AI-written opinion. No automatic setting changes.

User flow:

1. User inserts Mix Buddy on a Logic Pro track.
2. Plugin captures a bounded audio sample or feature summary from the track.
3. User can type extra context/questions in the plugin UI before sending, such as:
   - "Is this vocal too harsh?"
   - "How should I EQ this bass?"
   - "Give me mix advice for a modern pop vocal."
4. Plugin sends the request to an AI provider.
5. Plugin displays the response as text.

AI provider direction:

- Primary path: OpenAI/ChatGPT API.
- Investigate whether Apple on-device AI can be used for this. Do not assume it can directly critique arbitrary music audio inside an audio plugin.
- If local Apple AI cannot accept the required audio input, use local DSP feature extraction plus an OpenAI request.
- Raw audio upload is allowed as a product direction for MVP exploration, but make it explicit in the UI and keep the implementation off the audio thread.

## MVP 2: Structured Parameter Suggestions

Goal: ask AI for concrete values that map to Mix Buddy's own plugin parameters.

Initial parameters may include:

- Input gain
- Output gain
- EQ band frequency, gain, and Q
- Compressor threshold, ratio, attack, release, and makeup gain
- Pan
- Short text notes explaining the recommendation

Implementation rules:

- Prefer structured JSON responses from the AI.
- Parse recommendations into a local model before touching parameters.
- Show suggestions to the user before applying.
- Applying changes updates Mix Buddy's own JUCE parameters.
- Do not assume Mix Buddy can directly control Logic Pro's native channel strip, stock EQ, fader, pan, sends, or other third-party plugins.

## MVP 3: Supervisor Bus Prototype

Goal: add a supervisor instance, likely on the stereo output or main bus, that coordinates multiple track instances.

Expected model:

1. Track instances publish local summaries and current Mix Buddy parameter state.
2. A supervisor instance listens to the whole mix on the bus.
3. The supervisor asks AI for whole-mix recommendations.
4. The supervisor dispatches text or structured parameter instructions to track instances.
5. Track instances show and optionally apply approved changes to their own JUCE parameters.

Important limitation:

- Do not assume Logic Pro exposes full project/session state to one plugin.
- Inter-instance communication, a local coordinator service, or explicit user-driven export/import may be needed.

## Architecture Boundaries

Keep these concerns separate:

- `PluginProcessor`: real-time-safe audio pass-through, parameter state, metering, and capture buffers.
- `Analysis`: local feature extraction such as peak/RMS, loudness, spectrum, dynamics, stereo width, silence, and transient information.
- `AIClient` / `OpenAIClient`: async request construction, upload, response parsing, retries, and errors.
- `MixModel`: track summaries, plugin parameter values, AI recommendations, and apply decisions.
- `Supervisor`: session-level coordination across plugin instances.
- `PluginEditor`: UI controls, prompt input, status, response display, and apply buttons.

Network requests, file I/O, model calls, and blocking work must never run on the audio thread.

## Real-Time Audio Rules

The audio callback must avoid:

- Blocking network or file I/O.
- Allocating memory in the hot path where avoidable.
- Locks that may block unpredictably.
- Logging from the audio thread.
- Calling UI or API code directly.

Use preallocated buffers, atomics, lock-free or bounded queues, and background workers/message-thread handoff.

## API And Privacy Rules

- Do not hard-code API keys.
- Do not commit secrets.
- Prefer user-owned credentials.
- Store credentials using macOS Keychain or another secure local mechanism.
- Clearly show when audio or extracted features are being sent externally.
- Keep AI responses advisory by default until the user explicitly applies changes.

## Near-Term Priorities

1. Replace the placeholder "Hello World" UI with a simple Mix Buddy shell.
2. Add basic metering to prove real audio is flowing through the plugin.
3. Add a text input for the user's prompt/context.
4. Add a safe track audio capture path or feature extraction path.
5. Add an async OpenAI client behind a narrow interface.
6. Display AI response text in the plugin UI.
7. Add structured recommendation parsing and plugin-owned parameters.
8. Prototype inter-instance/supervisor communication only after single-track analysis works.

## Coding Guidance

- Keep changes small and buildable.
- Prefer JUCE and existing CMake patterns already in the repo.
- Keep DSP, UI, networking, and model code in separate modules once they become non-trivial.
- Preserve CLion compatibility.
- Add external dependencies only when there is a clear reason.
- Add comments only for non-obvious audio, threading, host, or API behavior.
