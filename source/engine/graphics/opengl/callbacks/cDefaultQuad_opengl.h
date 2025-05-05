#pragma once

#include "engine/graphics/opengl/assets/cQuad_opengl.h"
#include "engine/graphics/opengl/cShader_opengl.h"

namespace df::opengl::render_callbacks
{
	class cDefaultQuad_opengl
	{
	public:
		static void forwardQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad );

		static void deferredQuad( const cShader_opengl* _shader, const cQuad_opengl* _quad );
		static void deferredQuadFinal( const cShader_opengl* _shader, const cQuad_opengl* _quad );
	};
}
