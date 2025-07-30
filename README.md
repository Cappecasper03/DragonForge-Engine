# DragonForge Engine

**Note: This project is a work in progress.**

A modern C++ game engine designed with flexibility and performance in mind, featuring a modular multi-renderer architecture.

## Features

- Modern C++23 codebase
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
| OpenGL   | Full      | Full support |
| Vulkan   | Partial   | Basic support, limited features |
| DirectX  | Planned   | Planned for future |
| Metal    | None      | No current plans |

## Requirements

- C++23 compatible compiler
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

## Note

Most parts of this documentation were generated with the assistance of AI tools.