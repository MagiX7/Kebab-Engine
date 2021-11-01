# Kebab Engine

## Description

This is an 3D Game Engine made for the subject Game Engines at CITM - UPC Terrassa (2021) by David González and Carlos Megia.

The objective is to create a useful 3D Engine that fits all the basic aspects to create a 3D game and to learn how to do and how an Engine works.

## 3rd Party Links

- [SDL](https://www.libsdl.org/)
- [OpenGL](https://www.opengl.org/)
- [ImGUI](https://github.com/ocornut/imgui)
- [DevIL](http://openil.sourceforge.net/)
- [Assimp](https://github.com/assimp/assimp)
- [MathGeoLib](https://github.com/juj/MathGeoLib).

## Team Members

- [Carlos Megia](https://github.com/Chuchocoronel).

- [David González](https://github.com/MagiX7).

## How to use

#### Create new gameobjects:

- On GameObject section there're some geometries you can create on the scene.
- Drag and drop the fbx, it will load the mesh and the texture (if it has one, on diffuse chanel and it comes with the model).
  * If the fbx not comes with the texture and you want to put it on the model, you will have to left click the gameobject that has the mesh where you want that texture on, and drag and drop the texture.

#### Things you can do on the gameobjects:

- Left click on the gameobject on the panel hierarchy and on inpector panel it will show the name, position, rotation, scale (where you can move the gameobject) and the mesh and texture if it has one. If it has childs, it will display under, on the hierarchy panel.
  * If the gameobject has mesh you can show the normals of vertex or triangles and change their colors.
  * If the gameobject has a texture component you can show the default checkers texture instead of the original texture.
- Right click on the gameobject on the hierarchy panel and you can delete the gameobject and it's childs (if it has), create a new empty child and move up/down on the hierarchy if there're other gameobjects.

#### Camera controls

- Right click + Move the mouse - Camera rotation.
- Right click + WASDQE - Camera movement.
  * W - Forward.
  * S - Backward.
  * A - Left.
  * D - Right.
  * E - Up.
  * Q - Down.
- Gameobject clicked + F - Center gameobject on the camera.
- Gameobject clicked + Alt + Left Click - Orbit around the gameobject.
- P - Save.
- L - Load.
- M - Show/Hide Wireframe.

## Additional Features

- Camera functionality by frustum.
- Frame buffer to show the scene on a imgui window.
- Docking.
- Show texture preview on inspector panel.
- Calculated bounding box AABB for each gameobject.
- Camera as a component.
