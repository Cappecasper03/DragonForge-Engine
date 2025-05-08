# DragonForge Engine

A modern C++ game engine designed with flexibility and performance in mind, featuring a modular multi-renderer architecture.

## Features

- Modern C++20 codebase
- Multi-renderer architecture
- ImGui integration
- Cross-platform support
- Built-in profiling tools
- Asset management system
- Event system
- Input handling

## Platform Support

| Platform | Status     | Notes |
|----------|------------|-------|
| Windows  | Full      | Primary development platform |
| Linux    | Partial   | Basic support, limited features |
| MacOS    | None      | No current plans |

## Renderer Support

| Renderer | Status     | Notes |
|----------|------------|-------|
| OpenGL   | Full      | Primary rendering backend |
| Vulkan   | Partial   | Basic support, limited features |
| DirectX  | Planned   | Planned for future |
| Metal    | None      | No current plans |

## Requirements

- C++20 compatible compiler
- [Xmake](https://xmake.io/) build system

## Building

### Windows

1. Clone the repository:
```bash
git clone https://github.com/yourusername/DragonForge-Engine.git
cd DragonForge-Engine
```

2. Generate project files:
- Run `scripts/GenerateProjectFiles.ps1`

## Build Configurations

- Debug: Development with full debug information
- Release: Fully optimized release build
- Profiling: Optimized build with profiling enabled

## Project Structure

- `/source`
  - `/application` - Main application layer
  - `/engine` - Core engine systems
  - `/shaders` - Shader files
- `/scripts` - Build and utility scripts
- `/xmake` - Build system configuration and utilities
- `/game` - Binary output directory
- `/build` - Intermediate build files

## Dependencies & Licenses

| Library | License |
|---------|---------|
| Assimp    | BSD 3-clause License |
| fmt     | MIT License |
| FreeType  | FreeType License (BSD-style) |
| GLM       | MIT License |
| SDL3      | Zlib License |
| STB       | MIT License / Public Domain |
| Vulkan Loader | Apache License 2.0 |
| Vulkan Headers | Apache License 2.0 |
| Vulkan-HPP | Apache License 2.0 |
| Vulkan Validation Layers | Apache License 2.0 |
| VMA-HPP (Vulkan Memory Allocator) | MIT License |
| Tracy Profiler | BSD 3-clause License |
| GLAD      | MIT License |
| Dear ImGui | MIT License |
| Slang     | MIT License |

## Note

Most parts of this documentation were generated with the assistance of AI tools.