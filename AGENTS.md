# AGENTS.md

These are the working rules for agents in this repo. JAGE (Just Another Game Engine) is a small C++17 game engine (MIT) on SDL, OpenGL 4.1 core and spdlog. It goes 2D first, then 3D.

## Sources of truth

- `README.md`: the pitch, current status and build steps.
- `ROADMAP.md`: decisions already made, the milestones, and what is out of scope. Check it before proposing features. Respect those decisions unless the owner reopens them.
- Work from the next unchecked item in `ROADMAP.md`. Don't build past the current milestone without asking.
- Before stopping, tick finished items and record new decisions in `ROADMAP.md`.

## Layout

- `jage/`: the engine, a static library. Public headers are in `jage/include/jage/` and sources in `jage/src/`. Namespaces follow folders: `jage::core`, `jage::graphics`, `jage::input`, `jage::managers`.
- `jageeditor/`: the app that runs the engine. Its `main()` only calls `Engine::Run()`, which holds the test quad until M2 moves the demo to `samples/sandbox`. `jageeditor` comes back as the editor in M6.
- `external/`: third-party code. GLAD is generated source. SDL2 (headers, plus Windows binaries) and spdlog are vendored until M1 fetches dependencies at configure time.
- `premake5.lua`, `premake/`, `tools/`, `cli.*`, `package.json`: the old build and commit tooling. M1 removes them.
- `docs/brand/`: the logo. `docs/screenshot.png`: the README screenshot.

## Engine rules (hard)

- **OpenGL 4.1 core is the ceiling**, because it is the highest version macOS supports. No GL 4.2+ calls and no GLSL above `#version 410 core`.
- **Set SDL GL attributes before the window is created.** SDL's docs require it: on Windows and X11 the pixel format (depth, stencil, MSAA, sRGB) is fixed when the window is created. The context version and profile are read at `SDL_GL_CreateContext`.
- **Destroy GL objects before the context.** Meshes, shaders and textures must not outlive `Engine::Shutdown()`.
- **Check GL calls in Debug** with `JAGE_CHECK_GL_ERROR` after each call, as the existing graphics code does.
- **Every macro exists in every configuration, and every call ends with `;`.** A macro in `log.h` or `helpers.h` that is compiled out in Release still needs a Release definition, and a call without its `;` only compiles in Debug. Once M0 makes Release compile, build Debug and Release before calling a change done.
- **Include engine headers by their full path**, `"jage/…"`. Short paths such as `"graphics/mesh.h"` stopped resolving in `54458a4` and broke the build.
- **Use the exact define names:** `JAGE_PLATFORM_WINDOWS`, `JAGE_PLATFORM_LINUX`, `JAGE_PLATFORM_MACOS`, `JAGE_CONFIG_DEBUG` and `JAGE_CONFIG_RELEASE`.
- **SDL stays behind the engine.** Public headers don't include SDL. Forward-declare SDL types, as `window.h` and `joystick.h` do. Games use `jage::` APIs only.
- **Demo code belongs in an app, not in `jage/`.** `Engine::Run()` still holds the original test quad until M2 moves it out. Don't add more.

## Code style

- Classes and methods are `PascalCase`. Members are `snake_case_` with a trailing underscore. A few older names (`Engine::instance()`, `programId_`, `glContext_`) predate this; rename them when you touch the file. File names are lowercase with no separators (`rendermanager.h`). Macros start with `JAGE_`.
- Braces go on their own line. Headers use `#pragma once`.
- Formatting: match the file you are editing until `.clang-format` lands in M1. After that, run clang-format.
- New public API gets a short `/** */` doc comment. Today only `window.h`, `keyboard.h` and `mouse.h` have them. Everywhere else, comment why, not what: a non-obvious constraint, a workaround and its cause, or an SDL or GL quirk. No commented-out code or change notes.

## Commands

Until M0 lands, the last commit only builds with workarounds, on Windows, from a Developer PowerShell for Visual Studio in the repository root:

- `.\premake\premake5.exe vs2022` generates `jage.sln`.
- `$env:CL = '/I..\jage\include\jage /D_ITERATOR_DEBUG_LEVEL=0'` works around the broken includes and the bundled fmt that Visual Studio 2026 rejects.
- `msbuild jage.sln /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145` builds Debug. On Visual Studio 2022, drop the toolset flag. Release doesn't compile.
- Copy `external\sdl2\lib\SDL2.dll` into `bin\Debug\jageeditor\`, then run `.\bin\Debug\jageeditor\jageeditor.exe`.
- The `CL` workaround hides the include bug instead of failing on it. Don't use it when checking an M0 fix.

## Brand

- The assets are in `docs/brand/`. `-dark` files are for dark backgrounds.
- The mark is a pixel-art isometric cube in cornflower blue (`#6495ED`), the renderer's default clear colour. The wordmark is a 5×7 pixel font drawn as rectangles. There is no font to load, so edit the SVG paths directly.

## Working style

- **Commits:** [Conventional Commits](https://www.conventionalcommits.org/) (`feat:`, `fix:`, `docs:`, `chore:`, `test:`, `ci:`, `build:`, `refactor:`), atomic, with a scope when it adds clarity (`fix(input): …`, `build(cmake): …`). Commits before 2026 used git-cz with emoji. New commits don't.
- **No AI attribution** in commits or PRs. That means no `Co-Authored-By` trailers, no "Generated with" lines and no session links.
- **Checks:** automate acceptance checks instead of handing manual steps to the owner. Give every check that tests for an absence a positive control. For example, the GL 4.1 guard must fail on a deliberate `glDispatchCompute` call.
- **Docs:** short and concise. Prefer editing `ROADMAP.md` over creating new planning documents. When the stack changes, update the README, this file and the GitHub topics in the same milestone.
- **Validation:** evidence comes from building sample games on the engine, and after release from public signals (issues, forks, games made). Don't plan interviews or outreach.
