# Roadmap

JAGE is a small C++ game engine, revived in 2026 after two dormant years. This file tracks what gets built, in what order, and the decisions already made. Every milestone ends in something that runs.

## Decisions (2026-09-25)

- **2D first, then 3D.** v0.1.0 is a playable 2D sample game made with the engine and its editor. Models, lighting and materials come after it. The renderer stays 3D-capable throughout: transforms are 3D even for sprites, and one camera type covers both projections.
- **CMake replaces premake.** CMake with presets replaces premake, the Python CLI and the checked-in binaries. SDL3 and spdlog are fetched at configure time at pinned tags. The last premake build stays reachable at the `legacy-premake` tag.
- **SDL3 replaces SDL2.** SDL2 only gets maintenance fixes, and SDL3 has been stable since January 2025. The port happens now, while the engine is under 2,000 lines.
- **OpenGL 4.1 core is the only renderer**, because it is the highest version macOS supports. GLAD is regenerated for 4.1 core, so calls from later versions don't compile.
- **Samples drive the engine.** From M2 on, each milestone builds or grows a sample: a sandbox, Pong, Breakout (which the editor and play mode then open), a platformer, a starter template, a glTF viewer, then a 3D diorama. A library choice (audio, ECS, map format, physics) is made in the milestone whose sample first needs it, and recorded here.
- **Credit.** JAGE's code so far is derived from episodes 1 to 11 of progrematic's *Let's Make an Engine* series and its [Hippo](https://github.com/progrematic/hippo) engine (MIT). `LICENSE` keeps progrematic's copyright notice and adds the current one, and the README credits the series.
- **Brand** is a pixel-art isometric cube in cornflower blue (`#6495ED`), the renderer's default clear colour, with a 5×7 pixel wordmark drawn as rectangles. The assets are in `docs/brand/`, with `-dark` files for dark backgrounds.
- **Commits** are plain Conventional Commits. git-cz, its emoji and the Node files it needs leave in M1.
- **History was rewritten** to drop the git-cz emoji from every commit subject and to put every commit under the current author. Code, dates and message text are otherwise unchanged, but every commit hash changed, so clones from before 2026-09-25 need a fresh `git clone` or `git reset --hard origin/main`.

## M0: Revival (as soon as possible)

- [x] Rewrite the README: status, a screenshot, today's build steps with their workarounds, and credits.
- [x] Add this roadmap, `AGENTS.md`, and a `CLAUDE.md` that contains `@AGENTS.md`.
- [x] Add `.gitattributes` with LF line endings.
- [x] Add the pixel cube logo to `docs/brand/`.
- [x] Keep progrematic's copyright notice in `LICENSE` and add the current one.
- [x] Set the GitHub description and topics.
- [ ] Make the last commit compile again:
  - [ ] include engine headers as `"jage/…"` everywhere. Commit `54458a4` changed the engine include directory, and five sources still include headers by their old paths (`"graphics/mesh.h"`, `"log.h"`);
  - [ ] make every log, assert and GL-check macro a single statement in both configurations, give `JAGE_ASSERT` a Release definition, and end every call with `;`. Today about 80 calls leave out the `;`, which only compiles in Debug;
  - [ ] update the vendored spdlog. Its bundled fmt 9.1.0 uses `stdext::checked_array_iterator`, which MSVC 14.51 (Visual Studio 2026) removed.
- [ ] Copy `SDL2.dll` next to `jageeditor.exe` after the build.
- [ ] Add premake's BSD license file next to its binaries, which ship without it.
- [ ] Drop the `CL` workaround from the README and `AGENTS.md`.
- [ ] Tag the result `legacy-premake`.

**Done when:** from a clean checkout, `premake5 vs2022` and `msbuild` build Debug and Release with only the toolset override, the Debug exe is still running 5 seconds after it starts, and `git ls-remote --tags origin` lists `legacy-premake`.

## M1: Build and CI

- [ ] CMake build:
  - [ ] the `jage` static library with a `jage::jage` alias, and the `jageeditor` app;
  - [ ] `CMakePresets.json` with `debug`, `release` and `ci` presets;
  - [ ] SDL3 and spdlog through `FetchContent` at pinned release tags;
  - [ ] `JAGE_BUILD_SAMPLES` and `JAGE_BUILD_TESTS` options, off when JAGE is added as a subproject;
  - [ ] warnings at `/W4` and `-Wall -Wextra`, treated as errors in CI for JAGE's own targets;
  - [ ] on Windows, the SDL3 DLL is copied next to each executable after the build (`$<TARGET_RUNTIME_DLLS>`).
- [ ] Regenerate GLAD with glad2 for `gl:core=4.1` plus `GL_KHR_debug`. The current loader was generated for 4.6.
- [ ] Port to SDL3: init, events, the window and GL context, and `SDL_GameController` to `SDL_Gamepad`.
- [ ] Remove premake (`premake5.lua`, `premake/`, `external/glad/premake5.lua`), the Python CLI (`cli.*`, `tools/`), `external/sdl2`, `external/spdlog`, and `package.json` with its lockfile. Drop their lines from `.gitattributes` and the allow list from `.gitignore`, and ignore the presets' build directory.
- [ ] Fix the rest of the 2026-09 audit:
  - [ ] `shader.h` uses `uint32_t` without including `<cstdint>`, which GCC rejects;
  - [ ] gamepads: axes are stored as `bool`, so `GetAxis` returns only 0 or 1; `GetAxis` calls `abs` on a float without `<cmath>`; and `JoyStick::Shutdown` erases from the map with an invalidated iterator, and nothing calls it;
  - [ ] a removed gamepad is matched by device index, but SDL reports removals by instance ID. After a reconnect, an unplugged pad stays in slot 0 and the next one is stored under `-1`;
  - [ ] premake defines `JAGE_PLATFORM_MACOS`, but the code tests `JAGE_PLATFORM_MAC`, so the macOS paths never compile in;
  - [ ] the GL attributes are set after the window is created, against SDL's docs. The profile and version still apply, because SDL reads them when it creates the context, but on Windows and X11 SDL fixes the pixel format (depth, stencil, MSAA, sRGB) when it creates the window;
  - [ ] resizing the window never updates the GL viewport. Size it from the window's pixel size, for high-DPI displays;
  - [ ] `Window::Create` carries on after a failed `SDL_CreateWindow`, ignores the GLAD load result, and never deletes the GL context;
  - [ ] a shader that fails to link sets its `uint32_t` program id to `-1`, and later calls pass that id to GL;
  - [ ] indexed meshes draw `GL_TRIANGLES`, but meshes without indices draw `GL_TRIANGLE_STRIP`.
- [ ] Add doctest under CTest, with first tests for the axis scaling and deadzone math.
- [ ] Add `--frames N`: run N frames with vsync off, then exit 0. In Debug, a GL error or a failed assert exits non-zero, and a CTest case with a deliberate GL error checks that it does.
- [ ] Add a `.clang-format` that matches the current style (tabs, braces on their own line), and format the tree once.
- [ ] CI on GitHub Actions:
  - [ ] build and test Debug and Release on Windows (MSVC), Linux (GCC and Clang) and macOS (AppleClang), with dependency caching;
  - [ ] run `jageeditor --frames 120` in Debug on Linux under Xvfb and on Windows, both with Mesa's llvmpipe software OpenGL;
  - [ ] macOS builds and runs unit tests only, because hosted macOS runners draw OpenGL with Apple's software renderer, which has known rendering bugs.
- [ ] Update the README build steps to use the presets, and swap the `sdl2` GitHub topic for `sdl3` and `cmake`.

**Done when:** CI is green for Debug and Release on all three platforms, `ctest` passes, `jageeditor --frames 120` exits 0 on the Linux and Windows jobs, `git ls-files` lists no premake, Python, Node or prebuilt binary files, and a deliberate call to a GL 4.3 function such as `glDispatchCompute` fails to compile.

## M2: Game loop

- [ ] Decide first, and record under Decisions:
  - [ ] C++17 or C++20;
  - [ ] how games reach engine services: an `EngineContext&` passed to callbacks, or global accessors;
  - [ ] the error policy. The recommendation is asserts for programmer errors, `std::optional` or a small `Result<T>` for runtime failures, and no exceptions across the public API.
- [ ] Replace the hard-coded loop in the `Engine` singleton with a `jage::Application` interface (`OnInit`, `OnFixedUpdate`, `OnUpdate`, `OnRender`, `OnShutdown`) and a `jage::Run(app, config)` entry point. Games are separate executables that link `jage::jage`.
- [ ] Move the demo out of `Engine::Run()` into `samples/sandbox`. `jageeditor` stops building until M6 brings it back as the editor.
- [ ] `EngineConfig`: the window title and size, vsync, the frame limit behind `--frames`, and a hidden window for tests. Under `--frames`, each frame runs exactly one fixed step, so frame N shows the same state on every machine.
- [ ] Loop: one `Tick()` per frame, so SDL3's main callbacks or a web build can drive it later. A fixed 60 Hz update, variable-rate rendering with an interpolation alpha, and a clamp on long frames, timed with `SDL_GetTicksNS`.
- [ ] Events: SDL events become typed engine events (close, resize, focus, key, text input, mouse, gamepad added and removed) and go through a layer stack, so the debug overlay and the editor can sit above the game later.
- [ ] Input is built from engine events only, never read from SDL directly, so tests and replays can inject it:
  - [ ] `enum class` values replace the int key and button codes;
  - [ ] pressed, held and released queries per frame for keys, mouse buttons and gamepad buttons.
- [ ] A test attaches an SDL3 virtual gamepad and checks hot-plug, buttons and the deadzone.
- [ ] CI checks that no header in `jage/include/` includes SDL. A throwaway header that does must fail the check.

**Done when:** CTest passes on all three platforms, including loop tests with a fake clock (a given elapsed time produces the expected number of fixed updates) and input tests driven by synthetic events, and `samples/sandbox --frames 600` exits 0 on the Linux and Windows jobs.

## M3: Sprites

- [ ] Decide first, and record under Decisions: the world conventions. The recommendation is right-handed, Y-up, column-major matrices, and a pixels-per-unit value per texture, pinned by tests.
- [ ] glm for vectors and matrices, and a `Transform` (position, quaternion rotation, scale) that sprites use too.
- [ ] Asset paths resolve from the executable's folder (`SDL_GetBasePath`). A missing file fails with an error that names the resolved path.
- [ ] Shaders load from `.glsl` files under `assets/shaders/`. A compile error names the file and the line.
- [ ] Meshes take a vertex layout (position, UV, colour, normal) instead of positions only.
- [ ] Textures load with stb_image. Filtering defaults to nearest, for pixel art. A failed load shows a placeholder texture and logs the path.
- [ ] One `Camera` type with orthographic and perspective projections. 2D uses orthographic, with pan and zoom.
- [ ] Render targets: offscreen framebuffers with colour and depth, used for screenshots now and for the editor viewport in M6.
- [ ] Render queue: draw items sorted by pass, layer, shader and texture, and a per-frame camera uniform buffer. `RenderManager` moves into `jage/src/graphics/`, and only that folder calls OpenGL. A CI check enforces it, and a throwaway `glClear` call outside the folder must fail it.
- [ ] Sprite batch renderer: textured, tinted and rotated quads, sub-regions of a sprite sheet, and sorting layers, all in one render pass.
- [ ] Renderer statistics: draw calls and texture binds per frame.
- [ ] Sandbox: a few hundred sprites in a seeded layout. One sprite moves with the keyboard or a gamepad, and the mouse pans and zooms the camera. `--stress` draws 10,000 sprites from one atlas.
- [ ] Sample art is CC0 (for example Kenney) or made for the project. Each asset folder records its source and license.
- [ ] Test tools:
  - [ ] `--screenshot N:path` writes frame N to a PNG with stb_image_write;
  - [ ] a CTest check compares a screenshot with a reference image within a small per-channel tolerance, and CI uploads a diff image when it fails;
  - [ ] reference images are generated on the Linux CI image (`ubuntu-24.04` with a pinned Mesa), and one script regenerates them.

**Done when:** the sandbox's frame-60 screenshot matches its reference on Linux CI, `samples/sandbox --stress --frames 300` exits 0 with at most 10 draw calls per frame as the statistics report, and the CI check finds no `gl*` calls outside `jage/src/graphics/`.

## M4: Pong

- [ ] Decide first, and record under Decisions: miniaudio or SDL3 audio for sound, and stb_truetype glyph atlases or a bitmap font from a sprite sheet for text.
- [ ] Input actions: games bind named actions such as `paddle_up` to keys, gamepad buttons or axes.
- [ ] Seeded random numbers owned by the engine. They don't use `std::uniform_*_distribution`, whose output differs between standard libraries.
- [ ] Deterministic simulation: builds use `/fp:precise` and `-ffp-contract=off`, and never fast-math. Simulation code avoids libm functions such as `std::sin` and `std::atan2`, whose results differ between C runtimes.
- [ ] Input recording and replay: `--record path` stores each fixed step's action state, the random seed and a hash of the final game state. `--replay path` exits non-zero when the replayed state differs, and a recording with one changed input must fail.
- [ ] Virtual resolution: render to a fixed-size target, then scale it to the window with integer scaling and letterboxing.
- [ ] Text rendering for scores and menus.
- [ ] Audio: load and play WAV and OGG sound effects, with a volume control. OGG goes through stb_vorbis, because neither miniaudio nor SDL3 decodes it alone. Headless runs use a null audio device.
- [ ] 2D box overlap and sweep helpers.
- [ ] Pong sample: a title screen, two players (keyboard and gamepad), first to 5, and a sound on every hit. Paddles and ball are plain structs; there is no entity system yet.

**Done when:** in CI, a recorded match replays to the state it recorded on both the Linux and Windows jobs and exits 0, the changed-input recording fails, the final frame matches the reference image, and a test plays a sound through the null audio device.

## M5: Breakout

- [ ] Decide first, and record under Decisions:
  - [ ] EnTT or a small ECS of our own, and whether games see its types or a thin `Scene` and `Entity` wrapper;
  - [ ] the scene file format. The recommendation is versioned JSON with nlohmann/json;
  - [ ] how scene files refer to assets: by path or by ID;
  - [ ] how components describe their fields to the serializer and the M6 inspector: `entt::meta` or hand-written field lists.
- [ ] Scenes: entities with stable IDs and a `Name`, a parent-child hierarchy on `Transform`, and `SpriteRenderer` and `Camera` components. Systems run in fixed phases, in a declared order.
- [ ] Scene files save and load with a version field. A malformed or unknown-version file fails with an error that names the file and the field, and doesn't crash.
- [ ] Scene switching between frames, for the title, level, pause and game-over screens.
- [ ] Asset handles: `Handle<T>` with an index and a generation, so a stale handle is caught instead of reading freed memory. Textures, sounds and fonts load once per path.
- [ ] Breakout sample: three levels stored as scene files, lives and a score. The paddle follows the mouse or a gamepad.

**Done when:** a scene saved, loaded and saved again is byte-identical, the malformed-file and stale-handle tests pass, and in CI a replay clears level 1 and exits 0.

## M6: Editor

- [ ] Dear ImGui (docking branch) with its SDL3 and OpenGL 3 backends, as a layer on the M2 layer stack, scaled for high-DPI displays.
- [ ] A debug overlay that any game toggles with F1: frame-time graph, renderer statistics and entity count.
- [ ] `jageeditor` returns:
  - [ ] a dockspace with a saved default layout;
  - [ ] a viewport panel that renders the scene into an M3 render target, with its own editor camera;
  - [ ] hierarchy: create, delete, rename and reparent entities;
  - [ ] inspector: edit components, and add or remove them;
  - [ ] a log console fed by an spdlog sink, with a level filter;
  - [ ] a File menu with New, Open and Save;
  - [ ] `jageeditor <scene> --save <path>` opens a scene and saves it on exit, so CI can check the round trip.

**Done when:** `jageeditor` opens a Breakout level with `--frames 300 --save out.scene`, exits 0, and `out.scene` is byte-identical to the input.

## M7: Play mode

- [ ] Decide first, and record under Decisions: how game code runs inside the editor. The default is native C++ behaviours, registered by name and linked into a per-project editor build. The alternatives are a shared library or Lua.
- [ ] A native script component with `OnCreate`, `OnUpdate` and `OnDestroy`. Breakout's game logic moves into script components, so the Breakout sample and play mode run the same code.
- [ ] Play, Pause, Step and Stop. Play runs on a copy of the scene, and Stop restores it.
- [ ] A standalone runtime loads a project's start scene with no editor code.

**Done when:** a test enters play mode, replays a recording for 300 fixed steps, stops, and confirms the scene equals its state before play, and the same recording reaches the same state in the standalone Breakout build and in play mode.

## M8: Platformer

- [ ] Decide first, and record under Decisions: Tiled or LDtk for maps, and Box2D v3 or hand-written tile collision for the player.
- [ ] Tilemaps: load layers from the map editor's files, draw them with the sprite batch, collide with a solid layer, and support one-way platforms.
- [ ] Sprite animation: clips of sprite-sheet frames and a small state machine (idle, run, jump, fall).
- [ ] Camera follow with a dead zone and level bounds, plus parallax background layers.
- [ ] Music: stream a looping track. Music and effects have separate volumes.
- [ ] A fullscreen toggle and a pause menu.
- [ ] Platformer sample, built in the editor: one level with coins, hazards, a checkpoint and a goal flag. Coyote time and jump buffering live in the sample, not the engine.

**Done when:** in CI, a replay reaches the goal flag within 3,600 fixed steps and exits 0 on the Linux and Windows jobs, and the level's map file loads in a doctest test.

## M9: v0.1.0

- [ ] Decide first, and record under Decisions: supported platforms and minimum compilers, and versioning (SemVer, where 0.x releases may change the API).
- [ ] Core journey: `templates/starter` is a game that pulls in JAGE with `FetchContent` at a release tag. Before the first release, CI points it at the commit under test. Clone it, build it with a preset, run it, and a sprite moves. The README quick start is this template.
- [ ] A getting-started guide: make a small game from the starter template.
- [ ] Errors a newcomer can act on:
  - [ ] without OpenGL 4.1, a message box says so;
  - [ ] a failed SDL init or a missing asset names the cause and the next step;
  - [ ] logs also go to `jage.log` next to the executable.
- [ ] The version comes from `project(VERSION)` in CMake, replacing the hard-coded `v0.1.0` window title and `v0.1` log line. It is logged at startup and shown in the editor.
- [ ] The window icon is the brand mark.
- [ ] README: the quick start, supported platforms, and a screenshot or GIF of each sample. macOS stays marked as not run-tested until someone runs a release on a Mac.
- [x] `LICENSE` (MIT).
- [ ] `THIRD_PARTY_NOTICES.md` for fetched libraries, sample assets and the Hippo origin.
- [ ] `CONTRIBUTING.md`: building, tests, recording a replay, adding a sample, and commit messages.
- [ ] `SECURITY.md`: reports go through GitHub private vulnerability reporting, which is turned on in the repository settings. Asset loaders are not hardened against untrusted files.
- [ ] Issue tracker: bug and feature templates (the bug template asks for the OS, GPU, driver and `jage.log`), labels including `critical` and `good first issue`, and a few good first issues.
- [ ] Fresh-install check: a CI job on clean runners follows the README quick start step by step.
- [ ] Release workflow: a `v*` tag builds the editor and the platformer for Windows, Linux and macOS, zips each with its assets, and publishes them to a GitHub Release with SHA-256 checksums. Builds are unsigned.
- [ ] Release check: CI downloads the Linux and Windows zips and runs the platformer replay from them.
- [ ] Release notes, generated from the Conventional Commits history, list what works and the known limitations.

**Done when:** CI is green on `main`; a `v0.1.0-rc.1` tag produces a draft GitHub Release with builds and checksums for all three platforms; the fresh-install job and the release check pass; a script confirms that `LICENSE`, `CONTRIBUTING.md`, `SECURITY.md`, `THIRD_PARTY_NOTICES.md` and the issue templates exist; and `gh issue list --label critical --state open` returns nothing. Then `v0.1.0` is tagged from the same commit.

## M10: First 3D

- [ ] Decide first, and record under Decisions: glTF's metallic-roughness PBR without image-based lighting, or Blinn-Phong, for the first materials. glTF import covers static meshes now, and skins later.
- [ ] Load glTF 2.0 with cgltf: meshes with normals, UVs and tangents, embedded and external textures, and nodes as entities. The results are assets behind handles.
- [ ] Material assets: a shader plus parameters and textures, sorted in the render queue by shader and material.
- [ ] A 3D opaque pass beside the 2D pass, with depth testing and back-face culling, and a `MeshRenderer` component on the same `Transform` and hierarchy as sprites.
- [ ] One directional light, sent in a uniform buffer.
- [ ] A perspective orbit camera, used by the editor viewport and by `samples/viewer`, which loads a glTF model.

**Done when:** golden images for three models from Khronos's glTF-Sample-Assets, named in the test and with their licenses recorded, pass on Linux CI; a malformed glTF file fails with an error that names the file; a scene with mesh, material and light components round-trips byte-identical; and every 2D reference image and replay from M3 to M8 still passes without regenerating.

## M11: Diorama

- [ ] Up to eight point lights as components, in the same uniform buffer.
- [ ] Linear lighting and simple tone mapping in the 3D pass, written through an sRGB framebuffer. The 2D pass keeps its current colour handling, so its reference images still match.
- [ ] A perspective fly camera.
- [ ] Sprites and text draw over the 3D scene as a HUD.
- [ ] 3D box collision helpers.
- [ ] Diorama sample: walk through a small CC0 glTF room in first person and collect three items.

**Done when:** the diorama's frame-60 screenshot matches its reference on Linux CI, a replay collects all three items and exits 0, and every 2D reference image and replay from M3 to M8 still passes without regenerating.

## Later

- Shadow maps, a skybox, HDR and bloom, then image-based lighting.
- Mipmaps, anisotropic filtering (through `GL_EXT_texture_filter_anisotropic`, which is only core from GL 4.6), MSAA, instancing and frustum culling.
- Skeletal animation from glTF skins.
- 3D physics with Jolt, and a small 3D game to prove it.
- Editor: undo and redo, mouse picking in the viewport, ImGuizmo gizmos, an asset browser, and prefabs.
- Particles, and a settings screen (volumes, key rebinding, fullscreen) saved under `SDL_GetPrefPath`.
- Hot reload for shaders and assets, by swapping the resource behind its handle.
- Profiling with Tracy, behind a `JAGE_PROFILE` option.
- Lua scripting through sol2, if writing game code in C++ becomes the bottleneck.
- Packing a game's assets into one archive.
- A job system for loading and simulation. OpenGL calls stay on the main thread.
- A web build with Emscripten, which needs a WebGL 2 shader path.
- Spatial audio, and navigation with Recast and Detour.
- Game UI with RmlUi, if menus outgrow sprites and text.
- API reference with Doxygen.
- Renovate with a custom regex manager for the pinned `FetchContent` tags (Dependabot can't read CMake), clang-tidy, and a coverage report.

## Not planned

- Hand-written Vulkan, Metal or Direct3D 12 backends. OpenGL 4.1 core covers all three desktop platforms. If macOS drops OpenGL, the path is the SDL3 GPU API behind the same render queue.
- OpenGL features above 4.1, such as compute shaders and direct state access. macOS stops at 4.1.
- Console and mobile ports. Console SDKs are under NDA and can't live in a public MIT repo. Mobile needs OpenGL ES, touch input and store packaging.
- Networking and multiplayer in the engine. A game that needs it can use ENet or GameNetworkingSockets directly.
- Our own physics engine or math library. Box2D v3, Jolt and glm cover these.
- Visual scripting. Games are written in C++.
- premake, SDL2 or the Python CLI after M1. The last premake build stays at the `legacy-premake` tag.
- Production-engine guarantees. There is no API stability promise before 1.0. To ship a commercial game, use Godot.
- An asset store or plugin marketplace.

## How we'll know it works

Evidence comes from dogfooding. Every sample links only the public `jage::jage` target, so a sample that needs a private header shows the boundary is wrong. Each engine gap a sample hits becomes an item in this file. When 3D lands in M10 and M11, the 2D reference images and replays must still pass unchanged, which shows the foundation held. After v0.1.0, a second small game made from the starter template, such as a jam entry, should need engine fixes, not rewrites. After release, the evidence also comes from public signals: issues, pull requests, forks and games people make with JAGE.
