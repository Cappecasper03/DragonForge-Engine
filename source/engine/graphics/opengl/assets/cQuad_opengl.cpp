#include "cQuad_opengl.h"

#include "cTexture_opengl.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/graphics/opengl/cShader_opengl.h"
#include "engine/graphics/vulkan/cRenderer_vulkan.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/callbacks/cDefaultQuad_opengl.h"
#include "engine/graphics/opengl/OpenGlTypes.h"

namespace df::opengl
{
	cQuad_opengl::cQuad_opengl( std::string _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		DF_ProfilingScopeCpu;

		vertex_array.bind();

		vertex_buffer.bind();
		vertex_buffer.setData( sizeof( sVertex ) * m_vertices.size(), m_vertices.data(), cBuffer_opengl::kStaticDraw );

		index_buffer.bind();
		index_buffer.setData( sizeof( unsigned ) * m_indices.size(), m_indices.data(), cBuffer_opengl::kStaticDraw );

		vertex_array.setAttribute( 0, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::position ) );
		vertex_array.setAttribute( 1, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::tex_coord ) );
		vertex_array.unbind();

		texture = new cTexture_opengl( fmt::format( "{}_{}", name, "texture" ), cTexture_opengl::k2D );
	}

	bool cQuad_opengl::loadTexture( const std::string& _file_path, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		return texture->load( _file_path, _mipmapped, _mipmaps, _flip_vertically_on_load );
	}

	void cQuad_opengl::render()
	{
		DF_ProfilingScopeCpu;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader_opengl >( cQuadManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cShader_opengl >( render_callback, this );
		else
			cRenderCallbackManager::render< cShader_opengl >( cQuadManager::getDefaultRenderCallback(), this );
	}

	iRenderCallback* cQuad_opengl::createDefaults()
	{
		DF_ProfilingScopeCpu;

		iRenderCallback* callback;

		if( cRenderer::isDeferred() )
			callback = cRenderCallbackManager::create( "deferred_quad", render_callbacks::cDefaultQuad_opengl::deferredQuad );
		else
			callback = cRenderCallbackManager::create( "forward_quad", render_callbacks::cDefaultQuad_opengl::forwardQuad );

		return callback;
	}
}
