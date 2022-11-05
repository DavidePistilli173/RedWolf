![alt text](https://github.com/DavidePistilli173/RedWolf/blob/devel/RedWolf/logo/RedWolf.png?raw=true)

### ********** INTRODUCTION **********

RedWolf is a library for console and GUI applications.
It is developed using Microsoft Visual Studio 2022. The intention is to make the library cross-platform at least with Windows and Linux, 
but at the time of writing the only compiler which supports all used C++20 features is MSVC.

    IMPORTANT NOTE:
        Due to the number of templated member functions available, using a pre-compiled  
        version of the library could lead to ABI breaking (due to different STL implementations  
        in different compilers or compiler versions).
        Because of this, RedWolf MUST be recompiled with the user's build-chain before use.

### ********** CODE STRUCTURE **********
All the code for the RedWolf library is contained inside the RedWolf directory.
This contains the following sub-directories:
    
    - alg -> Algorithms;
    - cont -> Containers;
    - core -> Core classes;
    - dat -> Simple structures;
    - events -> Base and default events;
    - io -> Input/Output classes;
    - lif -> Library InterFaces for third_party dependencies;
    - net -> Network;
    - text -> String and text processing;
    - third_party -> Third-party dependencies;
    - thread -> Multi-threading;
    - time -> Date and time;
    - ui -> User Interace
    - utils -> Utilities.

### ********** COMPILATION **********

In order to compile the library, you need to install the Vulkan SDK from https://vulkan.lunarg.com/ (The currently tested version is 1.3.216.0).
GLFW sources are already included in the CMake project and are located in the third_party folder.
GLM sources are already included in the CMake project and are located in the third_party folder.

You must first build the RedWolf library project, then RedWolfSandbox.

### ********** DOCUMENTATION **********

The documentation can be generated using Doxygen and the Doxyfile located in the root folder of the project.
