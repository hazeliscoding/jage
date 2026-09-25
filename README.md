<h1>
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="docs/brand/lockup-dark.svg">
    <img alt="JAGE" src="docs/brand/lockup.svg" height="48">
  </picture>
</h1>

**Just another game engine.** A small C++ engine on SDL and OpenGL, built one system at a time to learn how every part of a game engine works.

Engine tutorial series tend to stop halfway, and production engines are too big to learn from. JAGE began as a follow-along of one of those series, and its roadmap goes past where the series ended: sprites, scenes, audio, an editor, then 3D. Each system stays small enough to read in one sitting, and each milestone ends in a sample that runs.

> **Status:** revival. JAGE was dormant from mid-2024 until September 2026, and the last commit doesn't build as it stands: a half-finished include change broke five files, and Release has macro bugs on top. With the workarounds under [Build](#build), it opens a window, reads keyboard, mouse and gamepad input, and draws one shader-coloured quad. M0 gets the build compiling again, and M1 moves it to CMake and SDL3 and clears the rest of the revival audit. See [ROADMAP.md](ROADMAP.md).

<img alt="The JAGE window: a cornflower-blue background with one quad blending between green, yellow, pink and navy corners" src="docs/screenshot.png" width="600">

The quad's colours shift with the mouse, the arrow keys, and a gamepad's D-pad and left trigger.

## What works today

- **Window:** an SDL2 window with an OpenGL 4.1 core context, loaded through GLAD.
- **Input:** keyboard and mouse state with held, pressed and released checks, and hot-plugged gamepads. Gamepad axes read as 0 or 1, and unplugging a pad can go unnoticed, until M1 fixes both.
- **Rendering:** meshes (VAO, VBO and EBO), shaders with cached uniform locations, and a queue of render commands flushed once per frame.
- **Diagnostics:** spdlog logging macros, asserts that break into the debugger, and a GL error check after almost every GL call in Debug.

## Where it's going

- **M0 Revival:** the last commit compiles again, and the premake build is tagged and retired.
- **M1 Build and CI:** CMake and SDL3, CI on Windows, Linux and macOS, and the rest of the audit's bug fixes.
- **M2 Game loop:** an application API, a fixed time step, and input built from typed events.
- **M3 Sprites:** textures, a camera, a render queue and a sprite batch.
- **M4 Pong:** input actions, text, audio and recorded replays.
- **M5 Breakout:** entities, scene files and asset handles.
- **M6 Editor:** Dear ImGui with a viewport, hierarchy and inspector.
- **M7 Play mode:** script components, and play and stop inside the editor.
- **M8 Platformer:** tilemaps, animation, a follow camera and music.
- **M9 v0.1.0:** release builds, a starter template and a getting-started guide.
- **M10 First 3D:** glTF models, materials and a light.
- **M11 Diorama:** point lights, a first-person camera and a 3D sample.

Each milestone and its "done when" check are in [ROADMAP.md](ROADMAP.md).

## Build

The last commit doesn't build without the workarounds below. M0 fixes the code, and M1 replaces these steps with CMake presets on all three platforms.

These steps are verified from a clean checkout on Windows 11 with Visual Studio 2026. Run them from a Developer PowerShell for Visual Studio, in the repository root:

```powershell
.\premake\premake5.exe vs2022
$env:CL = '/I..\jage\include\jage /D_ITERATOR_DEBUG_LEVEL=0'
msbuild jage.sln /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145
copy external\sdl2\lib\SDL2.dll bin\Debug\jageeditor\
.\bin\Debug\jageeditor\jageeditor.exe
```

- The `/I` flag finds the headers that five files include by their old paths.
- `_ITERATOR_DEBUG_LEVEL=0` gets past the fmt 9.1.0 copy bundled with spdlog, which uses `stdext::checked_array_iterator`, and Visual Studio 2026 removed it.
- The `vs2022` projects ask for the v143 toolset, and Visual Studio 2026 ships v145. On Visual Studio 2022, drop `/p:PlatformToolset=v145`.
- Release doesn't compile.
- Linux used `premake/premake5.linux gmake2` and macOS `premake/premake5 gmake2`, both with a system SDL2. Neither has been tested since 2024.

## Credits

JAGE began as a follow-along of progrematic's [*Let's Make an Engine*](https://www.youtube.com/playlist?list=PL7lh9ryRNHSIzqKzEdYPG94B0uvfqhHpb) series. Its code so far is derived from episodes 1 to 11 of the series' [Hippo](https://github.com/progrematic/hippo) engine (MIT, Copyright (c) 2021 progrematic), with renames, doc comments and newer dependencies. The roadmap goes past where the series ended.

## Contributing

The revival is reshaping the build, so pull requests will likely conflict until M1 lands. Issues are welcome. A contributor guide arrives with v0.1.

## License

[MIT](LICENSE). The notice keeps progrematic's original copyright.
