# OpenGL Graphics Engine
![Mandelbrot and Julia set](https://i.imgur.com/9mzZoWT.png)
## About
This engine was created to have a quick and easy setup for an OpenGL context as well as being able to quickly render primitives.
### Libraries
- OpenGL (https://www.opengl.org/) (https://www.khronos.org/opengl/)
- GLM (https://github.com/g-truc/glm)
- Glad loader (https://glad.dav1d.de/)
- GLFW (https://www.glfw.org/)
- Matrix Stack (Jason L. McKesson)
## Features
### Implemented
#### Rendering
- Camera
- Simple Translations, Scale, and Rotations using a matrix stack
- Loading custom shaders
##### Primitives
- Plane
- Triangle
#### Shaders
- Default shader: basic colour, used to test rendering works
- Mandelbrot Set Shader
- Julia Set Shader
#### User Input
#### Keyboard
- A-Z keys
- Arrow Keys
- Shift and Control keys
##### Mouse
- Mouse Coordinates
- Mouse Buttons (left, right, middle mouse)

### Todo
#### Rendering
- Render functions for primitives without new instance of primitives.
- Textures
- Skybox
##### Lighting
- Blinn-Phong lighting
- Reflections
##### Primitives
- Cube
#### Shaders
- Toon shader
- Ray marching shader
#### User Input
- More keys
- Mouse movement
