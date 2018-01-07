
# Veritas Engine
This is a small barebones game engine that I work on to experiment with C++ 11/14/17. 

This a personal project I work on in my spare time aside from the work I do professionally.

# Features
There some features that have been written that are under iteration, however they do work

* A simple asset conditioning pipeline - Recursively iterates over a directory and exports models, animations, skeletons materials (using Assimp) and textures files (using NVTT) into a file format Veritas Engine can load and render.
* World Setup using JSON - Describe the scene using JSON to position of lights and models in the world.
* Renders models using diffuse, specular, and normal maps.
* Animates models and skeletons that assimp is able to export.
* Directional, point, and spot lights.
* Recursive scene graph for adding removing entities from the scene.
* Asynchronous resource loader for loading models, skeletons, and textures into the engine.
* A process manager.
* Small object allocator
* Compile time string hashing.
* Asynchronous job system with child and continuous jobs.

# External Dependencies

There are several 3rd party libraries the engine relies on including from vcpkg:

* Boost (Property map, Filesystem)
* Boost-DI
* SPDLOG
* Intel Thread Building Blocks
* Cereal
* NVidia Texture Tools (NVTT)
* Assimp
* GLM
* Miniz-CPP

# Running the Code

TODO

# Sources
Parts of the code are inspired from several sources

## Books

* Game Engine Architecture by Jason Gregory
* Game Coding Complete by by Mike McShaffry and David Graham
* Modern C++ Design by Andrei Alexandrescu
* Effective Modern C++ by Scott Meyers
* Introduction to 3D Game Programming with DirectX 11 by Frank Luna

## Online Resources

* DDSTextureLoader by Microsoft (https://github.com/Microsoft/DirectXTK/wiki/DDSTextureLoader)
* Miniz-CPP (https://github.com/tfussell/miniz-cpp)
* Jill model and Cat Model from turbo squid (http://www.turbosquid.com)
* RasterTek (http://www.rastertek.com/)
* Molecular Musings Blog (Special thanks for the tutorials on a multithreaded job system: https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/)

# Architecture

## Packages

![Package Diagram](http://i.imgur.com/hjZL1DR.png)

### VeritasEngine

This is a dynamic linked library that is that main interface the game executable talks to. It exposes interfaces for the process manager, job manager, animation manager, scene graph, and resource manager. This code is platform agnostic and compiles using Clang. This links with VeritasEngine_Windows to hook up a DirectX renderer. 

### VeritasEngine_Windows

This is a static lib that is linked into VeritasEngine. Its goal is to contain all platform specific code. It contains a series of Impl units that hook up the rendering bits of the engine as well as any additional platform specific code.

### VeritasEngineBase

This is a static lib that provides shared facilities amongst the VeritasACP and VeritasEngine. Examples include reading files from disk, the small object allocator. Additonally it has the types that are serialized and deserialized in the ACP and engine.

### VertiasACP

This is an application that iterates over a directory and creates resource files that can be loaded by the engine. It currently exports models, skeletons, animations, materials, specular maps, diffuse maps, and normal maps. Additional types will be added as the engine adds support for them.

# Future Work

* Event System
* Scripting/Console interface
* Height maps
* Integrate physics engine
* more when I get there....
