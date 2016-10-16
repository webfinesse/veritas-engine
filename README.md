# Veritas Engine
This is a small barebones game engine that I work on to experiment with C++ 11/14/17. 

This a personal project I work on in my spare time aside from the work I do professionally.

# Features
There some features that have been written that are under iteration, however they do work

* A simple asset conditioning pipeline - Recursively iterates over a directory and exports models (using Assimp) and textures files (using NVTT) into a file format Veritas Engine can load and display.
* World Setup using JSON - Describe the scene using JSON to position of lights and models in the world.
* Renders models using diffuse, specular, and normal maps.
* Directional, point, and spot lights
* Recursive scene graph for adding removing entities from the scene.
* Recursive resource loader to load models, and textures into the engine.
* A process manager.

# External Dependencies

There are several 3rd party libraries the engine relies on including:

* Boost 1.60+ (Property tree, Filesystem)
* Cereal
* NVidia Texture Tools (NVTT)
* Assimp
* GLM
* Miniz-CPP

# Sources
Parts of the code are copied/inspired from several sources

## Books
