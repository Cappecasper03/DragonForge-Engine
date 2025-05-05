#pragma once

#include "engine/graphics/opengl/assets/cMesh_opengl.h"
#include "engine/graphics/opengl/cShader_opengl.h"

namespace df::opengl::render_callbacks
{
	class cDefaultMesh_opengl
	{
	public:
		static void forwardMeshAmbient( const cShader_opengl* _shader, const cMesh_opengl* _mesh );
		static void forwardMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh );

		static void deferredMesh( const cShader_opengl* _shader, const cMesh_opengl* _mesh );
	};
}
