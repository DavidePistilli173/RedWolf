![alt text](https://github.com/DavidePistilli173/RedWolf/blob/devel/RedWolf/logo/RedWolf.png?raw=true)

INTRODUCTION
    RedWolf is a library for console and GUI applications.
    It is developed using Microsoft Visual Studio 2022. The intention is to make the library cross-platform at least with Windows and Linux, 
    but at the time of writing the only compiler which supports all used C++20 features is MSVC.

    IMPORTANT NOTE:
        Due to the number of templated member functions available, using a pre-compiled version of the library could lead to ABI breaking (due
        to different STL implementations in different compilers or compiler versions).
        Because of this, RedWolf MUST be recompiled with the user's build-chain before use.

COMPILATION
    In order to compile the library, you need to install the Vulkan SDK from https://vulkan.lunarg.com/ (The currently tested version is 1.3.216.0).
    GLFW sources are already included in the CMake project and are located in the third_party folder.
    GLM sources are already included in the CMake project and are located in the third_party folder.

DOCUMENTATION
    The documentation can be generated using Doxygen and the Doxyfile located in the root folder of the project.
