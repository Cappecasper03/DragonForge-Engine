#include "cQuad_opengl.h"

#include "cTexture2D_opengl.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/opengl/callbacks/cDefaultQuad_opengl.h"
#include "engine/graphics/opengl/cShader_opengl.h"
#include "engine/graphics/opengl/OpenGlTypes.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cQuad_opengl::cQuad_opengl( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color )
		: iQuad( _name, _position, _size, _color )
		, m_push_constant( cBuffer_opengl::kUniform )
	{
		DF_ProfilingScopeCpu;

		m_vertex_array.bind();

		m_vertex_buffer.bind();
		m_vertex_buffer.setData( sizeof( sVertex ) * m_vertices.size(), m_vertices.data(), cBuffer_opengl::kStaticDraw );

		m_index_buffer.bind();
		m_index_buffer.setData( sizeof( unsigned ) * m_indices.size(), m_indices.data(), cBuffer_opengl::kStaticDraw );

		m_vertex_array.setAttribute( 0, 3, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::position ) );
		m_vertex_array.setAttribute( 1, 2, kFloat, sizeof( sVertex ), offsetof( sVertex, sVertex::tex_coord ) );
		m_vertex_array.unbind();

		m_push_constant.bind();
		m_push_constant.setData( sizeof( sPushConstants ), nullptr, cBuffer_opengl::kDynamicDraw );
		m_push_constant.unbind();

		const cTexture2D::sDescription description{
			.name       = fmt::format( "{}_{}", m_name, "texture" ),
			.size       = cVector2u( 1 ),
			.mip_levels = 1,
			.format     = sTextureFormat::kRed,
			.usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination,
		};
		m_texture = cTexture2D::create( description );
	}

	bool cQuad_opengl::loadTexture( const std::string& _file_path, const bool _mipmapped, const int _mipmaps, const bool _flip_vertically_on_load )
	{
		DF_ProfilingScopeCpu;

		const std::string full_path = cFileSystem::getPath( _file_path );

		const cTexture2D::sImageInfo   image_info = cTexture2D::getInfoFromFile( full_path );
		const cTexture2D::sDescription description{
			.name       = fmt::format( "{}_{}", m_name, "texture" ),
			.size       = image_info.size,
			.mip_levels = 1,
			.format     = image_info.format,
			.usage      = sTextureUsage::kSampled,
		};

		delete m_texture;
		m_texture = cTexture2D::create( description );

		return m_texture->uploadDataFromFile( full_path, m_texture->getFormat(), _mipmaps, _flip_vertically_on_load );
	}

	void cQuad_opengl::render()
	{
		DF_ProfilingScopeCpu;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader_opengl >( cQuadManager::getForcedRenderCallback(), this );
		else if( m_render_callback )
			cRenderCallbackManager::render< cShader_opengl >( m_render_callback, this );
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
