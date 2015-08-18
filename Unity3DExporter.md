Copy the Unity3DExporter.cs file from the Tools directory to the Assets/Editor directory of your Unity3D project. In Unity3D a Wire3D menu will appear containing the menu item Exporter. Selecting the item will open a dedicated editor window. Enter a destination directory for the exported data and click 'Export'.

Note that only a small subset of Unity3D is exported. Don't expect to export any arbitrary scene and get the same result you see in the Unity3D editor. The shader of a Unity3D material cannot be parsed by an editor script like the exporter, so information on render state or how textures are used cannot be extracted. Furthermore the Wii does not support shaders anyways. So if the exported scene does not look like you want it to, you have to open the exported xml file and change the properties of a material manually.

The following data is handled by the exporter:

### Game Object ###
  * Name
  * Static
  * Layer

### Components ###
**Transform**
  * Position
  * Rotation
  * Scale
  * Hierarchy of child transforms
**Mesh Renderer**
  * Enabled
  * Materials
  * Lightmap index
  * Submesh index (via reflection)
**Mesh Filter**
  * Mesh
**Light**
  * Enabled
  * Type (point/directional/spot)
  * Range
  * Ambient color (from Render Settings)
  * Light color (times Intensity)
  * Culling mask
  * Lightmapping (Baked only light will not be exported)
**Camera**
  * Enabled
  * Clear Flags
  * Background
  * Culling mask
  * Projection (orthographic exported as FOV is 0)
  * Field of view
  * Near clipping plane
  * Far clipping plane
  * Viewport rect
  * Depth
**Skybox**
  * Material
**Rigid Body**
  * Mass
  * Is Kinematic
  * Drag (Bullet Physics value, does not correspond to Unity's PhysX)
  * Angular Drag (Bullet Physics value, does not correspond to Unity's PhysX)
**Character controller**
  * Enabled
  * Slope Limit
  * Step Offset
  * Center
  * Radius
  * Height
**Box collider**
  * Enabled
  * Center
  * Size
**Sphere collider**
  * Enabled
  * Center
  * Radius
**Capsule collider**
  * Enabled
  * Center
  * Radius
  * Height
  * Direction
**Mesh collider**
  * Enabled
  * Mesh
  * Convex

### Assets ###
**Mesh**
  * Positions
  * Normals
  * Uv
  * Uv2
  * Colors/Colors32
  * Triangles
  * List of submeshes
**Material**
  * Texture/Lightmap
  * Main color (if available)
**Texture**
  * Wrap mode
  * Filter mode
  * Aniso level
  * Max size
  * Format (all formats are converted to RGB(A) during export)
  * Generate mipmaps

### Settings ###
**Physics**
  * Gravity
**Render**
  * Skybox
    * Tint color
  * Fog
    * Color
    * Mode
    * Start
    * End