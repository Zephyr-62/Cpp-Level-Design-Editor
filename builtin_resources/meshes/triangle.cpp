
#include "Mesh.h"

class TriangleMesh : public Mesh {

	TriangleMesh() {
		base::Mesh(
			{
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			},
			{
				0, 1, 2
			}
		);
	}


};