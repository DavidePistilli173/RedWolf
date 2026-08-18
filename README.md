![alt text](logo/RedWolf.jpeg?raw=true)

# RedWolf

## INTRODUCTION

RedWolf is an engine for console and GUI applications.
It is written using C++23 and should be compatible with Linux (Wayland).

It was inspired by:

- Hazel Engine => https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT
- Kohi Engine => https://www.youtube.com/playlist?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj

## COMPILATION

First, install these dependencies using your package manager:

- dbus
- libxkbcommon
- wayland
- wayland-protocols
- libglvnd
- vulkan-devel

Just configure and build the project using CMake and one of the provided profiles.

## DOCUMENTATION

The documentation can be generated using Doxygen and the Doxyfile located in the root folder of the project.

## EXAMPLES

The `examples` folder contains sample programs that show how to use the RedWolf engine.
