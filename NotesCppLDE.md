
# Level Design Engine Tool


### TODO

- Renderer
	- First triangle
	- Camera fixed
	- Camera pan
- SceneObjects Hierarchy


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


