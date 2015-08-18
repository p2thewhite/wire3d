The samples serve as tutorials and should be read in ascending order, since they build upon concepts that are explained in preceding samples.
The following samples show how to use the framework:

### Sample0 - User Application ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample0%2Fsrc)) shows how to define your own platform independent Wire3D application. In particular it shows creation and/or usage of:
  * a Wire3D derived application
  * smart pointers

### Sample1 - Basic Rendering ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample1%2Fsrc)/[Video](http://www.youtube.com/watch?v=llQveiCUlWQ)) demonstrates how to create a transparent, textured cube and render it several times (without a scene graph) using different render states and a light. In particular it shows creation and/or usage of:
  * vertex- and indexbuffers
  * renderable geometry (consisting of vertex- and indexbuffers)
  * a 2D texture with mipmaps
  * a material applying textures to geometries
  * render states (alpha blending, material state) and a light
  * a camera including frustum setup and culling
  * matrices and vectors performing geometric transformations

### Sample2 - Scene Graph ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample2%2Fsrc)/[Video](http://www.youtube.com/watch?v=ItP0GHJqn3I)) demonstrates how to create and render a scene graph.
Scene graphs serve the purpose of organizing your 3D objects (together with render states, lights, effects and controllers) in a hierarchy. The design of the classes is explained in detail [here](http://downloads.gamedev.net/pdf/Eberly-3.pdf). In particular this sample shows creation and/or usage of:
  * nodes and spatial hierarchies using nodes and geometries (i.e. a scene graph)
  * render state and light handling in scene graphs
  * transformation of local coordinates to world coordinates
  * calculation of a visible set to be rendered

### Sample3 - Level of Detail ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample3%2Fsrc)/[Video](http://www.youtube.com/watch?v=u_Zy2TFZL8E)) demonstrates how to use discrete LODs in the scene graph. In particular it shows creation and/or usage of:
  * a discrete level of detail (DLod) node
  * wireframe state
  * manual binding of resources
  * orthographic projection and text meshes

### Sample4 - Multitexturing ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample4%2Fsrc)/[Video](http://www.youtube.com/watch?v=wcuDh3NFq54)) demonstrates how to blend individual textures together in a single rendering pass using multitexturing.
In particular it shows creation and/or usage of:
  * texture blend modes
  * single pass multitexturing

### Sample5 - Lighting ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample5%2Fsrc)/[Video](http://www.youtube.com/watch?v=6FewhqOKSVU)) demonstrates how to render objects using lights. For that purpose we create 2 cubes which are both lit by 2 point lights using the scene graph. Furthermore we create a plane which is lit by a spot light and rendered without using the scene graph. The positions of the lights are represent by small cubes which are colored using the light's color. In particular this sample shows creation and/or usage of:
  * a scene graph managing lights
  * direct rendering and scene graph rendering

### Sample6 - Custom Nodes ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample6%2Fsrc)/[Video](http://www.youtube.com/watch?v=CgI6WTl5AWs)) demonstrates how to create your own nodes to be used in the scene graph. This way you can extend the functionality of the system and suit it your own needs. We create a custom node that renders a lens flare effect and seamlessly plugs into the existing system. In particular this sample shows creation and/or usage of:
  * node derived classes
  * the run time type information (rtti) system

### Sample7 - Dynamic Buffers ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample7%2Fsrc)/[Video](http://www.youtube.com/watch?v=4tlhkP-1uRc)) demonstrates how to create and use a dynamic vertex buffer. On creation of the vertex buffer a usage type is passed, indicating how the buffer is going to be accessed. With this information the framework can make correct assumptions and use the hardware accordingly. In particular it shows creation and/or usage of:
  * types of buffer usage
  * updating renderer bound (i.e. hardware) buffers

### Sample8 - Bullet Physics Integration ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample8%2Fsrc)/[Video](http://www.youtube.com/watch?v=qDFjKriAeko)) demonstrates how to integrate the [Bullet](http://code.google.com/p/bullet/) physics engine into Wire3D. The sample creates a static floor, a dynamic stack of boxes and 2 balls that collide with the stack and cause it to collapse. In particular the sample shows creation and/or usage of:
  * Bullet dynamics
    * continuous collision detection
    * rigid bodies

### Sample9 - Custom Main ###
This sample ([Wii Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample9_WII%2Fsrc)/[Win Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample9_WIN32%2Fsrc)) demonstrates how to use the Wire framework in your own program without having to derive from it. The benefit of this is that you have complete control over the entry point and order of execution, however this breaks plattform independency unless you take care of that yourself. In particular the sample shows creation and/or usage of:
  * custom entry point
  * initialization of the Wire framework
  * Wire renderer

### Sample10 - Material Sorting & Batching ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample10%2Fsrc)) demonstrates drawcall batching of static and dynamic geometry as well as sorting materials for minimizing state changes and correct transparency/opaque geometry order. In particular the sample shows creation and/or usage of:
  * shared materials and render states
  * visible set sorting
  * drawcall batching
  * render statistics

### Sample11 - Input ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FSample11%2Fsrc)) demonstrates how to process inputs from the Wiimote and Nunchuk on the Wii or their emulated counterparts on the PC. In particular the sample shows creation and/or usage of:
  * input devices and extensions
  * handling connection and disconnection of devices and extensions during runtime

### Demo ###
This sample ([Source](http://code.google.com/p/wire3d/source/browse/#svn%2Ftrunk%2FSamples%2FDemo%2Fsrc)/[Video](http://www.youtube.com/watch?v=ec_3FjXzKoo)) demonstrates how to load and manipulate a scene. In particular the sample shows creation and/or usage of:
  * Importer library to load a scene
  * controllers attaching to scene objects to access and manipulate them