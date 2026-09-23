# Repository Update Policy

Only the project owner may submit changes to this repository. AI agents may provide guidance, implementation plans, code snippets, examples, reviews, and troubleshooting help for the next steps of the project, but the project owner is the only person who should apply, commit, or submit repository changes.

Agents working from this file should treat the repository as advisory-only unless the project owner explicitly updates this policy.

# Project Brief

This project is intended to become a C++ audio plugin for Logic Pro that helps mix individual tracks using the OpenAI/ChatGPT API. The long-term target is a plugin system where each track can host an instance that analyses and proposes or applies processing, while an overall supervisor can inspect the whole session context and coordinate a balanced mix.

The current repository is only the default CLion C++ starter project. No audio plugin framework, DSP code, OpenAI client, or Logic Pro integration has been added yet.

## Product Goal

Build an audio plugin for Logic Pro that can:

- Run on individual audio tracks as a normal plugin insert.
- Analyse track audio and track metadata where available.
- Communicate with an OpenAI API backend to request mix guidance.
- Suggest or automate settings such as gain staging, EQ, compression, panning, sends, and broad tonal balance.
- Provide a supervisor view that can reason about all participating track instances and coordinate an overall mix.
- Keep critical audio processing real-time safe and avoid network calls on the audio thread.

## Likely Plugin Direction

Logic Pro primarily supports Audio Units on macOS, so the first serious implementation should target an AU/AUv3 plugin. JUCE is likely the most practical framework for cross-format audio plugin development from C++, even if Logic Pro is the first host target.

Expected early technical direction:

- Use C++20 or newer where practical.
- Replace the starter executable with a plugin-oriented project structure.
- Prefer JUCE for plugin scaffolding, UI, host integration, parameters, and audio/MIDI plumbing unless there is a clear reason to use Apple Audio Unit APIs directly.
- Keep DSP code independent from UI and API networking code.
- Use CMake if possible, since the project was started in CLion, but follow the chosen framework's normal build conventions.

## Architecture Notes

The system should separate these concerns:

- `AudioProcessor`: real-time-safe audio processing, parameter state, metering, and analysis buffers.
- `Analysis`: feature extraction such as loudness, peak/RMS, spectral balance, dynamics, stereo width, silence, and transient information.
- `OpenAIClient`: async API communication, request shaping, response parsing, retries, and error reporting.
- `MixModel`: local representation of tracks, plugin instances, parameters, recommendations, and supervisor decisions.
- `Supervisor`: session-level coordination across track instances.
- `UI`: plugin controls, analysis status, suggestions, approval/apply flows, and API settings.

Network requests must never run on the audio thread. Any API call should be asynchronous and communicate back through thread-safe queues, atomics, locks kept outside real-time callbacks, or framework-supported message-thread mechanisms.

## Track And Supervisor Design

The intended model is:

- Each track plugin instance collects local analysis and exposes controllable mix parameters.
- Instances can identify themselves with a user-provided track name or generated instance ID.
- A supervisor can gather summarized state from all active instances.
- The supervisor asks the OpenAI API for whole-mix recommendations and sends track-specific instructions back to each instance.

Because ordinary Logic Pro plugins cannot freely inspect every track in a session by themselves, the supervisor feature will likely need one of these approaches:

- A dedicated supervisor plugin instance placed on the stereo output or a bus.
- Local inter-plugin communication between instances.
- A companion app or local service that receives analysis from all instances.
- Explicit user export/import of mix state if host limitations block direct communication.

Do not assume Logic Pro exposes full project/session data to a plugin. Verify host and Audio Unit limitations before designing around them.

## OpenAI/API Requirements

API integration should be designed around user-owned credentials:

- Do not hard-code API keys.
- Do not commit secrets.
- Store credentials using macOS Keychain or another secure local mechanism.
- Provide clear status when the API is unavailable.
- Keep API responses advisory by default; applying changes should be explicit unless the user enables automation.

For prompts and model requests, prefer structured JSON inputs/outputs describing measured audio features, current parameters, musical intent, and allowed actions. Avoid sending raw audio unless there is a clear product decision, privacy model, and technical reason to do so.

## Real-Time Audio Safety

Audio callbacks must avoid:

- Blocking network or file I/O.
- Memory allocation in the hot path where avoidable.
- Locks that may block unpredictably.
- Logging from the audio thread.
- Calling UI or API code directly.

Use preallocated buffers, lock-free or carefully bounded queues, and background workers for analysis/API tasks.

## Development Priorities

Suggested order of work:

1. Decide framework and plugin format, likely JUCE targeting AU for Logic Pro.
2. Convert the starter CLion executable into a minimal plugin that loads in a host.
3. Add simple per-track metering and analysis.
4. Add safe parameter controls for gain, EQ placeholders, compression placeholders, and panning.
5. Add an async OpenAI client behind a narrow interface.
6. Add a recommendation workflow that suggests settings without applying them automatically.
7. Prototype inter-instance or companion-supervisor communication.
8. Add the supervisor UI and whole-mix recommendation flow.
9. Harden persistence, permissions, errors, and real-time behavior.

## Repository State

Current files:

- `CMakeLists.txt`: default CLion CMake executable project.
- `main.cpp`: default CLion hello-world program.
- `AGENTS.md`: project guidance for future agents and development sessions.

Generated CLion/CMake build output lives under `cmake-build-debug/` and should not be treated as source.

## Coding Guidance

- Keep implementation changes small and testable.
- Prefer clear module boundaries over putting plugin, UI, networking, and DSP code into one file.
- Preserve real-time audio constraints even during prototypes.
- Add comments only where they clarify non-obvious audio, threading, or host-integration behavior.
- Before adding external dependencies, explain why they are needed and how they fit the plugin architecture.
- When changing build files, keep CLion compatibility in mind.

## Open Questions

These decisions still need to be made before implementation:

- Whether to use JUCE or Apple Audio Unit APIs directly.
- Whether the first prototype should be AU only or also support VST3/standalone for testing.
- How track instances and the supervisor should communicate.
- What level of automatic mix adjustment is acceptable versus suggestion-only workflows.
- Which OpenAI model/API shape should be used for the first prototype.
- Whether any raw audio will ever be sent to the API, or only extracted analysis features.
