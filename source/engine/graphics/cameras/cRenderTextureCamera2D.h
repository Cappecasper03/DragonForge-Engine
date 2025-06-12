#pragma once

#include "engine/graphics/assets/textures/cRenderTexture2D.h"
#include "engine/graphics/cameras/cCamera.h"

namespace df
{
	class cRenderTexture2D;

	class cRenderTextureCamera2D : public cCamera
	{
	public:
		DF_DeleteCopyAndMove( cRenderTextureCamera2D );

		cRenderTextureCamera2D( const sDescription& _description );
		~cRenderTextureCamera2D() override;

		virtual void createTexture( const cRenderTexture2D::sDescription& _description );

		static cRenderTextureCamera2D* create( const sDescription& _description );

		void bindTexture( const std::uint16_t _index, const std::uint16_t _binding ) const { m_textures[ _index ]->bind( _binding ); }

		const std::vector< cRenderTexture2D* >& getTextures() const { return m_textures; }

	protected:
		std::vector< cRenderTexture2D* > m_textures;
	};
}
