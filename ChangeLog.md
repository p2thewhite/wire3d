## v1.2 (`r717`) ##
  * Wire:
    * added Input System
    * added support for multiple vertex streams per mesh
    * added layer masks to nodes, lights and cameras
    * seperated Image (i.e. buffer) from Texture (i.e. properties)
    * seperated handling of vertex declarations from vertex buffers
    * added usage type `STATIC_DISCARD_ON_BIND` for textures
    * removed `VisibleObject` in favour of `RenderObject`
    * removed scene graph dependency from Renderer
    * improved drawcall batching of static (sub)meshes
    * improved performance of drawing submeshes on PCs with software vertex processing
    * added camera node
    * added light attenuation
    * added blending mode `BM_ADD` to Material
    * (Wii) added support for submeshes to displaylist handling
    * (Wii) fixed vertex cache issue
    * (Wii) fixed a bug in handling lights
    * (DX9) added support for HLSL shaders
    * (DX9) added support for multiple render targets

  * Importer/Unity3D exporter:
    * added import/export of collision shapes, rigid bodies and character controller
    * added import/export of skybox
    * added import/export of submeshes
    * added option to combine static meshes
    * added option to export vertex colors as 32bit
    * exporting lights
    * exporting camera
    * exporting fog from `RenderSettings`
    * added disk and memory statistics summary after export
    * using default values for undefined transformation parameters
    * fixed overwritten default color values in render state
    * fixed incorrect calculation of font texture size
    * fixed text exceeding specified maximum length divided by 6
    * fixed wrong filter mode being used when texture has no mipmaps
    * fixed Importer header files polluting global namespace
    * fixed memory leaks

  * Samples:
    * added Sample11 to show how to use the input system
    * added Game sample to demonstrate new features
    * rewrote Sample10 to better demonstrate batching

## v1.0.2 (`r464`) ##
  * Wire
    * draw call batching of static and dynamic geometry
    * text rendering
    * skybox node
    * `VertexBuffer` uses 32bit color (instead of 4 floats) now, so no more conversion at Bind() time. **Note**: Although interface changed, there are no changes for the user due to implicit conversion operators to/from ColorRGB(A).
    * `IndexBuffer` only uses 16bit indices, as Wii does not support 32bit indices anyways. **Note**: Interface only changed on `IndexBuffer::GetData()`, if you are accessing index buffers via operator[.md](.md) there are no changes necessary.
    * (PC) fixed renderer statistics bug when `ResetDevice()`/`DestroyAll()` was issued.
    * (PC) fixed NVIDIA PerfHUD getting stalled when pressing a key.
    * (Wii) fixed anisotropic texture filtering

  * Importer
    * leaking vertex data fixed
    * support for "Static" flagged transformations
    * fixed missing Spotlight parameters

  * Tools
    * [freetype2](http://www.freetype.org/freetype2/index.html) font engine added

  * Samples
    * updated Sample3, Sample10 and Demo to showcase new features

## v1.0.1 (`r407`) ##
  * Wire
    * debug build uses custom operator new/delete
    * unified handling of platform dependent index buffers
    * fixed `ResetDevice` issue when `D3DPOOL_DEFAULT` objects are used

  * Importer
    * turned importer into lib
    * handle render states that are defined per material (additionally to per node/leaf defined render states)
    * handle vertex and index buffer access usage types
    * keep record of number of imported materials, textures, nodes, geometries, vertex buffers, index buffers
    * fixed memory leak

  * Tools
    * added Bullet Physics lib to solution for wii and win32
    * added Unity3D exporter
    * fixed 3rd party lib gcc warnings

## v1.0 (`r391`) ##
  * Initial release