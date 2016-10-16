# Veritas Engine
This is a small barebones game engine that I work on to experiment with C++ 11/14/17. 

This a personal project I work on in my spare time aside from the work I do professionally.

# Features (So far)
There some features that have been written that are under iteration, however they do work

* A simple asset conditioning pipeline - Recursively iterates over a directory and exports models (using Assimp) and textures files (using NVTT) into a file format Veritas Engine can load and display
* World Setup using JSON - Describe the scene using JSON to position of lights and models in the world.
* Renders models using diffuse, specular, and normal maps
* Recursive scene graph for adding removing GameObjects from the scene
