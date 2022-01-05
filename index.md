
# Introduction

Kebab Engine is a video game engine for 3D games. It has been developed as an assigment for the subject of Game Engines in the third year at CITM-UPC's university degree of design and development of videogames. The objective of the subject is to create a game engine from zero using OpenGL, this is the final result.

Hope you like it and it's useful for your projects.

# Team members

## David Jesus González López

![David](/docs/Images/David_image.png)

### Programmer

[Linked In](https://www.linkedin.com/in/david-jes%C3%BAs-gonz%C3%A1lez-l%C3%B3pez-03a2041b1/)

[Github](https://github.com/MagiX7)

### Work on Engine

- 3D Models and textures importer (also supports drag & drop)

- Mesh & Material Components

- Gameobject Hierarchy

- Hierarchy Panel

- OpenGL full integration and abstraction

- Geometry & Textures Drawing

- Wireframe shapes

- Preview camera, game & scene panels

- Scene guizmos

- Dockspace

- Integration JSON parser, PhysFS, mmgr, Optick, Assimp, glew, ImGuizmo, ImGui & MathGeoLib

- Mousepicking

- Resource Manager

- Scene serialization

- Play/Pause/Stop simulation

- Shader Drawing

- Water Shader Demo

## Carlos Megia Zubillaga

![Carlos](/docs/Images/Carlos_image.png)

### Programmer

[Linked In](https://www.linkedin.com/in/carlos-megia-zubillaga-b64bb31b7/)

[Github](https://github.com/Chuchocoronel)

## Work on Engine

- Camera logic and controls

- AABB/Boundig Boxes

- GameObject Class

- Console Panel

- Inspector Panel

- Assets Panel

- Window Configurations

- Configuration & About Panels

- Icons

- Transform Component

- Integration DevIL

- Frustum Camera

- Camera Culling

- Panels Basic Logic and Organization

- Right Click Menus

- Quadtree

- All Multimedia (Trailer, Web, Demo, etc.)

# Core sub-systems

### GameObjects

Based on an entity and component system, every gameobject is unique and it could have different components attached to it: transform, mesh, material and camera, all inherit from a component base class.

All the game objects have a parent and it allow to have children, creating a tree structure and as a consequence, a hierarchy.

### Components

The base to create all the game objects, because all game objects have components:

- **Transform:** allow the gameobject to move around the scene with a local or global transformation.
- **Mesh:** enable to include 3D meshes to render on the scene.
- **Material:** give the ability to renderize 2D textures on the mesh of the gameobject and also shaders.
- **Camera:** allow to create a camera and renderize the in-game from that point of view.

### Frustum

The base for the camera viewport, it allows to modify easily the characteristics of the camera view, and also is the main part for the **Camera Culling** that discart gameobjects from rendering if they aren't inside the frustum/camera, optimizing the engine.

![Frustum](/docs/Images/Frustum_image.png)

### File resources manager and explorer

The engine controls all the things related with meshes, textures, shaders and scenes. The resource manager regenerate missing files, create custom files for optimization and use reference counting on all of the resources used by the gameobjects.

On the other way, the file explorer allows the user to navigate through the files, drag and use them to the scene or eliminate them if not necessary.

![File Explorer](/docs/Images/File_explorer_image.png)

### Quadtree

The quadtree is used for optimization in collisions, when a gameobject is set static enters the quadtree and when the cube is full it divides in 4 parts. The collisions only works on cubes that are inside the camera frustum and the other are discarted. In our engine is used for mouse picking raycast and frustum culling.

![Quadtree](/docs/Gifs/Quadtree_gif.gif)

### Bounding boxes 

Bounding boxes are a very important system to detect collisions between gameobjects and optimize things, like quadtree or camera culling. Using AABB, a container used to create boxes based on the mesh of the model, is how we create the bounding boxes or colliders.

### Scenes simulation and serialization

Kebab engine can save and load diferent scenes, using it's own custom format. Not only that, also can start/stop a simulation or advance frame by frame, being the start to create videogames.

# Shaders

For the third and last assigment we decided to create the Shader pipeline using OpenGL. All the new meshes with materials have a basic shader, from there you can modify them at your own risk.

Each shader comes in one single file with the extension .shader. There you can find both the vertex and the fragment shader, they are divided by using #type vertex or #type fragment.

There is a default shader, which has the minimum implementation to compile and it supports textures, so you can still change it through the editor.

By default each mesh has a 1x1 pixel texture, so you can modify the color in the editor (it is passed to the shader as a uniform). This is used because if the mesh does not have a texture, the sahder does not work and it renders all black.

The shader system allows you to refresh a shader when it is edited outside the engine. So you can work on it and, when you save, the engine will automatically detect checking each five seconds if the last modified date of the file changed, and the shader will automatically be recompiled and a message on the console will show to give feedback.

![Functionality](/docs/Gifs/ShaderFuncionality_gif.gif)

### Water demo

To test if the implementation of the shader system worked as expected, we were required to do a water simulation.

You can find the shader for it under the folder Shaders in the Resources folder with the name "wave".

This shader simulates a straight wave with foam at the top and deep blue at the bottom. In between, both colors interpolate with the mix method from GLSL.

In order to achieve the wave movement we used the wave equation, and on top of it we added some noise using a random/hash.

After all this we made the waves not go equally deep modifying the position with the sinus of position.zx.

For this shader you can edit the amplitude of the wave, the frequency of the waves on the plane and the speed of them.

We also provided four textures to give it more realism. You can change them through in the inspector panel. (In case you want to see it without textures, use the white texture). 

![Editors](/docs/Gifs/ShaderEditor_gif.gif)

# Video demo

<iframe width="560" height="315" src="" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

# Links

Download the last release of Kebab Engine [Here]()

Github link to the engine repository [Here](https://github.com/MagiX7/Kebab-Engine)

# License

*MIT License*

*Copyright (c) 2021 David González and Carlos Megia*

*Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:*

*The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.*

*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*
