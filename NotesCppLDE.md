
# Level Design Engine Tool

### Done


### TODO
* Summary
-- MVP --
- Gizmos rendering + controls
- Serialization -> resources
	- Resources creation
	- importer or exporter: obj, gltf
- Transform Hierarchy
	- Account for delete children, duplicate in place
- Viewport object selection

-- Nice to Have --
- Rendering: lights/shadows, textures
- Mass select -> duplicate/delete
- Material instances: 
	- material resource serves as a template
	- each object can override the default values
- Scene Cameras: override editor camera rendering and controls
- Performance: frustum + backface culling
- Resources: advanced view on inspector (render mesh/material)

-- Future Work --
- Mesh Editor!
- Procedural mesh generation

*Details
- Editor utilities
	- shared material/material instance --> resource instance? 
		- resources could serve as templates, which then provide logic to track changes?
	- create new resources?
		- material, mesh (not for now --> mesh editor), (not shaders)
	- Allow multiple selection in Hierarchy?
		- mass clone/delete
		- display common components in hierarchy?
			- this is a _tedious_ implementation
	
	- Scene grid rendering
	- Add gizmos controls (screen to scene ray projection)
		- Transform
		- Camera align with axis (+-x,y,z)
		- grid granularity
	- grid vertex snap
	- Merge Meshes? a parent gets a mesh resulting from the fusion of every child's mesh (how to bridge faces/edges?)

- Default meshes:
	- cube, 3-pyramid, 4-pyramid, 
	- procedural: circle, sphere, cone
- Import .glb meshes?

- Scene Hierarchy:
	- TransformChild calculation (& caching?)

- Camera SceneObject
	- Preview camera? --> whole renderer pipeline duplicate
	- Select to drive as main editor camera, then switch back (viewport button if EditorCamera.isOverriden)

- Serialization of Resources:
	- Mesh, material, 
	- Scene
		- Get object (by name/id)
		- Json serialization (define format) --> Should be handled by resource load/unload
		- Appliaction Loading scene
	- Resource Manager
		- Unload resources that are not being used

- Performance: Frustum + backface culling

- Resources inspectors:
	- Mesh: render in small view with default plain shader
	- Material: render in quad

- Rendering:
	- Lights SceneObjects (point, directional)
		- Shadows
	- Textures

- mesh editing? extrude faces, add edges...
- procedural mesh generation!!
	- grow from base mesh?

### Systems
- Renderer
	- Camera System
	- Light Rendering
	- Mesh Drawing
- Editor
	- Views: 
		- Mesh Editor
		- Scene View
			- Scene Tree
		- File System
		- Inspector Window
	- MeshEditing
		- Move vertex/edge/face
		- Subdivide Edge
		- Extrude face
	
	- Scene
		- Switch Cameras
		- Instantiate
		- Object Instancing
		- Navigation (Camera transform live input controls)
		- Scene Loading (glb, customScene format?)
- FileSystem
	- DataSerializer
		- Load/save
		- Mesh, scenes, 


* Data Structures
- SceneData (gltf/GLB)
	- Mesh Collection
	- Material Collection
	- Camera Collection

- Scene
	- SceneObject[]
	
- Scene Object
	- Transform (position, scale, rotation)
	- Component[]	
	
- Component: Abstract	

- MeshRenderer: Component
	- Mesh
	- MaterialId

- Camera: Component
	- Mode: FreeRoam/Orbit/Pan
	- Options: FOV, RenderDistance (0.1, 5000)	
	
- Light Source: Component
	- Point Light	

- Resource: Abstract
	- Format

- Mesh: Resource
	- Vertex Array
	- TriangleArray
	- UV Array?

- Material: Resource
	- Albedo: Color(rgba)
	- Emission: float
	- Texture?
	- Shader? --> localSpace/globalSpace/screenSpace texture sampling


* Functions
- OBJ importer (meshData only)
- Utils Class: List operations, extract data from classes, etc


* Mesh editor Options
- Define Meshes programatically
- Save edited mesh as prefab
- Subdivide Edge






# Data Classes

- Scene
	- SceneObject[]

- SceneObject
	- Transform
	- Component[]
	
- Component: Abstract
	- Paint UI logic
	- Hold pertinent data

- Transform: Component
	- Position, rotation (quat), scale

- MeshRenderer: Component
	- Mesh
	- Material (if empty use default rendering program 'triangle.vert/frag')

- Resource: Abstract
	- Save/load logic

- Mesh: Resource
	- Vertex/Indices/UV Arrays
	- VOA, VAB...

- Material: Resource (really basic for now, no PBR)
	- Shader
	- Albedo
	- Emission?
	
- Shader: Resource
	
	
- Camera: Component
	- FOV
	- Near/Far planes
	- Projection: iso/perspective
	- (TODO: provide preview in small window)
	- Set as main method
	
- Application:
	- Globals
	- Scene
	- Renderer
	- GUI/GL setup
	- Main loop
	
- ResourceManager
	- handles shared resources
- Renderer
	- Main Camera: near/far, fov., transform...
	- Walks the scene tree and uses every meshRenderer component
	
	
# Folder Structure

/root
	/assets        user data (scenes, custom resources)
	/src/
		/core        					Application, utils
		/scene       					Scene, SceneObject, Component, Transform
		/components  			MeshRenderer, Camera
		/resources   				Resource, ResourceManager, Mesh, Material, Shader
		/renderer    				Renderer
		/editor      					panels, editor camera, selection...
	/builtin_resources/     built-in data
		/meshes
			(e.g. cube, quad, sphere, 
		/materials
			/shaders
	

