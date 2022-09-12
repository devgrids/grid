# [Grid](https://github.com/devgrids/grid)

## Summary
Grid is a graphics engine developed with the OpenGL API.

## Getting Started
Grid has a single dependency: [cmake](http://www.cmake.org/download/), which is used to generate platform-specific makefiles or project files. Start by cloning this repository, making sure to pass the `--recursive` flag to grab all the dependencies. 

```bash
git submodule update --init --recursive
```
Now configure vcpkg, a Microsoft library that allows you to add external library dependencies in a very simple way.

```bash
.\vcpkg\bootstrap-vcpkg.bat
```
Install the following libraries with vcpkg:


```bash
.\vcpkg\vcpkg install physx:x64-windows
.\vcpkg\vcpkg install imgui[core,glfw-binding,opengl3-binding,docking-experimental]:x64-windows
.\vcpkg\vcpkg install imguizmo:x64-windows
.\vcpkg\vcpkg install openvr:x64-windows
```

## Dependencies

 Library                                                |Functionality           |
 ------------------------------------------------------ |----------------------- |
 [assimp](https://github.com/assimp/assimp)             |Mesh Loading            |
 [physx](https://github.com/NVIDIAGameWorks/PhysX)      |Physics                 |
 [glew](https://github.com/nigels-com/glew)             |OpenGL Function Loader  |
 [glfw](https://github.com/glfw/glfw)                   |Windowing and Input     |
 [glm](https://github.com/g-truc/glm)                   |OpenGL Mathematics      |
 [stb](https://github.com/nothings/stb)                 |Texture Loading         |
 [imgui](https://github.com/ocornut/imgui)              |Graphical User Interface|
 [imguizmo](https://github.com/CedricGuillemet/ImGuizmo)|Gizmo Manipulate        |
 [openvr](https://github.com/ValveSoftware/openvr)      |Virtual Reality         |
 [spdlog](https://github.com/gabime/spdlog)             |Logging                 |

Now generate a project file or makefile for your platform. If you want to use a particular IDE, make sure it is installed; don't forget to set the Start-Up Project in Visual Studio or the Target in Xcode.

```bash
# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 17 2022" -B "build"
```
