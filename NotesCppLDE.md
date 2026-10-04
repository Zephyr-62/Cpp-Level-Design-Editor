
# Level Design Engine Tool


### Done

### TODO

- Paint scene to FBO
	- Then paste it into Viewport
	- Change FBO size based on viewport
	- Add this logic to a new Window class or somewhere it should belong better than Application?

- Editor changes:
	- Fix windows not showing up where required
		- Viewport, resources, 
	- List resources divided into categories: Mesh, Shader, Material
	- Merge resource inspector/scene obj inspector --> change which one is being used
	- Paint actual scene into viewport window

- Renderer
	- Camera fixed
	- Camera panoramic/isometric
	- Add camera settings menu into viewport

- Editor utilities
	- Add Component
	- Add scene obejct
	- Rename scene object
	- Duplicate scene object
	- Delete scene object

- Camera SceneObject
	- Select to drive as main editor camera, then switch back
	
- Scene Hierarchy:
	- TransformChild calculation (& caching?)

- Serialization of Resources:
	- Mesh, material, 
	- Scene
		- Get object (by name/id)
		- Json serialization (define format) --> Should be handled by resource load/unload
		- Appliaction Loading scene
	- Resource Manager
		- Unload resources that are not being used

- Rendering:
	- Lights SceneObjects (point, directional)
		- Shadows
	- Textures


### Main Loop

- Render Screen
	- Paint current selected view
- Read Input 
	- Execute actions based on context (current active view, registered actions...)

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
	

