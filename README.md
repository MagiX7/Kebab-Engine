# Kebab Engine

![David](/docs/Images/Kebab_engine_image.png)

## Description

This is an 3D Game Engine made for the subject Game Engines at CITM - UPC Terrassa (2021) by David González and Carlos Megia.

The objective is to create a useful 3D Engine that fits all the basic aspects to create a 3D game and to learn how to do and works an Engine.

## 3rd Party Links

- [SDL](https://www.libsdl.org/)
- [OpenGL](https://www.opengl.org/)
- [ImGUI](https://github.com/ocornut/imgui)
- [DevIL](http://openil.sourceforge.net/)
- [Assimp](https://github.com/assimp/assimp)
- [MathGeoLib](https://github.com/juj/MathGeoLib)
- [Parson](https://github.com/kgabis/parson)
- [PhysFS](https://github.com/icculus/physfs)
- [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
- [Optick](https://github.com/bombomby/optick)
- [ImGuiTextEditor](https://github.com/BalazsJako/ImGuiColorTextEdit)

## Team Members

- [Carlos Megia](https://github.com/Chuchocoronel).

- [David González](https://github.com/MagiX7).

## How to use

### Create:

In the Main Menu bar, you can find a menu item named "Create" where you can create the following:

#### Lights

By default a directional light comes in the scene. In the inspector you can tweak the values such as the color (ambient, diffuse and specular) or the direction.

Point Lights can be also created. You can edit the colors (just like in the directional) and adjust the constant, linearity and quadratic properties of the light.

#### GameObjects

- On GameObject section there're some geometries you can create on the scene.
- Drag and drop the fbx, it will load the mesh and the texture (if it has one, on diffuse chanel and it comes with the model).
  * If the fbx not comes with the texture and you want to put it on the model, you will have to left click the gameobject that has the mesh where you want that texture on, and drag and drop the texture.

#### Things you can do on the gameobjects:

- Left click on the gameobject on the panel hierarchy and on inpector panel it will show the name, position, rotation, scale (where you can move the gameobject) and the mesh and texture if it has one. If it has childs, it will display under, on the hierarchy panel.
  * If the gameobject has mesh you can show the normals of vertex or triangles and change their colors.
  * If the gameobject has a texture component you can show the default checkers texture instead of the original texture.
- Right click on the gameobject on the hierarchy panel and you can delete the gameobject and it's childs (if it has), create a new empty child and move up/down on the hierarchy if there're other gameobjects.

#### Controls

- Right click + Move the mouse - Camera rotation.
- Right click + WASDQE - Camera movement.
  * W - Forward.
  * S - Backward.
  * A - Left.
  * D - Right.
  * E - Up.
  * Q - Down.
- Mouse Wheel - Zoom in/out camera.
- Gameobject clicked + F - Center gameobject on the camera.
- Gameobject clicked + Alt + Left Click - Orbit camera around the gameobject.
- P - Save.
- L - Load.
- M - Show/Hide Wireframe.
- F2 - Recalculate Quadtree.

#### Panel Controls

- Hierarchy:
  * Right Click - Menu.
  * Hold Left Click on GameObject and drag - Reparent.
  * Left Click - Select GameObject.

- Assets:
  * Right Click on asset - Menu.
  * Right Click on panel - Menu.
  * Middle Click on asset - Reimport Menu.

## Additional Features

- Camera functionality by frustum.
- Frame buffer to show the scene on a imgui window.
- Docking.
- Show texture preview on inspector panel.
- Calculated bounding box AABB for each gameobject.
- Camera as a component.
- Drag and drop of external files to the engine.
- Drag and drop from assets (models or textures) to scene.
- Translate, rotate and scale by guizmos.
- Textures changeables.
- All meshes using AABB.
- Frustum culling.
- Mousepicking.
- Scene serialization.
- Space partition by quadtree.
- Start/Stop/Pause of simulation.
- Custom files for models/meshes/textures/scenes.
- 2 frame buffers to show game and editor with their respective cameras on imgui windows and can visualize both at the same time.
- Game camera changeable.
- Resource management to optimize and show assets properly.
- Editable shaders.
- Lightning system.

### Water Demo

In order to get the water demo working, you will need to select the plane (the child with name "pPlane2") that comes by default (can be seen in the hierarchy) and, in the Inspector Panel, in the "Material" section, change the shader to the wave.shader.
Important to not change the name of the file, or otherwise it won't work as expected.

