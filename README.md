<h1 align="center">📖 42 SCOP</h1>

## Description

This project is an introduction to OpenGL. The objective is to create an app capable of rendering 3D wavefront obj file, and of applying them textures (only .BMP) by using C++ and OpenGL.<br/>

## Setup
Requirements: `OpenGL version >= 3.3`.<br/>

Path for objects and textures if you want to add yours : `scop/resources/obj` and `scop/resources/textures` <br/>

Args : `./scop [object] [texture]` without the file extension.

## Keys

`Escape`: Quit the program.<br/>
`WASD`: Move object.<br/>
`T` : Apply the texture.<br/>
`C`: Switch to free camera mode with the mouse.<br/>
`Z`: Switch to wireframe mode.<br/>

## How it looks

- without vt mapping in obj file:<br/>
`./scop teapot t_container`<br/>

<img src="resources/screenshots/teapot1.png"
     alt="teapot screenshot"
     style="float: left; margin-right: 10px;" /><br/>


You can press `t` to apply the texture :


<img src="resources/screenshots/teapot2.png"
     alt="teapot screenshot"
     style="float: left; margin-right: 10px;" />
<br/>

- With vt mapping in obj file:<br/>
`./scop backpack t_backpack`, then press `t` to apply the texture :<br/>

<img src="resources/screenshots/backpack.png"
     alt="backpack screenshot"
     style="float: left; margin-right: 10px;" />
<br/>

With the wireframe mode enable :


<img src="resources/screenshots/backpack_wireframe.png"
     alt="backpack screenshot"
     style="float: left; margin-right: 10px;" />
