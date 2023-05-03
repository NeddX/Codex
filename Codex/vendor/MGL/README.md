# MGL - Minimalistic OpenGL Abstraction Layer

MGL as the name suggests, is a minimalistic lightweight C++ library that provides a simple and easy-to-use abstraction layer for OpenGL. 
It is designed to make OpenGL programming more straightforward by encapsulating common functionality into simple, reusable classes.
I wrote this library for personal use when I was learning OpenGL and was constantly writing different OpenGL applications.

This is mostly inspired by Yan "The Cherno" Chernikov's OpenGL series.

# Features
- Simple Abstraction Layer: MGL provides a minimalistic OpenGL abstraction layer, making it easy to get started with OpenGL programming.
- Shader Loading: MGL makes it easy to load shaders, allowing you to quickly get up and running with custom shaders.
- Texture Loading: MGL uses stb_image so you can easily load pictures as textures, making it simple to add custom graphics to your OpenGL applications.
- Basic Renderer: MGL comes with a built-in, very basic renderer, allowing you to easily render 2D graphics foe demonstration purposes.
- Lightweight: MGL is designed to be lightweight meaning you can easily build it from source and modify it.

# Getting started
To use MGL in your project, simply include the ```MGL.h``` header file and link against the MGL library. 
MGL uses CMake as its build system, so it should be straightforward to integrate into your existing cmake project using CMake's ```sub_directory()``` command.

# Example
Check out the ```Sandbox``` folder where I use MGL to draw bunch of rotating cubes with a texture on them.

NOTE: MGL expects both fragment and vertex shaders in a single file like the example below.
```glsl
#shader_type vertex
#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec4 a_Colour;

out vec4 o_Colour;

void main()
{
  o_Colour = a_Colour;
  gl_Position = vec4(a_Pos, 1.0);
}

#shader_type fragment
#version 330 core

in vec4 o_Colour;
out vec4 FragColour;

void main()
{
  FragColour = o_Colour;
}
```

# License
MGL is released under the MIT License. 
See the LICENSE file for more information.
