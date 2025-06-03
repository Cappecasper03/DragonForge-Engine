#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/cTexture2D.h"
#include "engine/graphics/types/sTextureFormat.h"
#include "engine/graphics/types/sTextureParameter.h"

namespace df::opengl
{
	class cTexture2D_opengl final : public cTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D_opengl );

		cTexture2D_opengl();
		~cTexture2D_opengl() override;

		void uploadData( const void* _data, sTextureFormat::eFormat _format, unsigned _mip_level = 0, bool _generate_mipmap = true ) override;

		void setInteger( sTextureParameter::eName _name, sTextureParameter::eParameter _param ) const;
		void setInteger( sTextureParameter::eName _name, const sTextureParameter::eParameter _param[ 4 ] ) const;

		void bind( int _index = 0 ) const override;
		void unbind( int _index = 0 ) const override;

		unsigned get() const { return m_id; }

	protected:
		void initialize( const sDescription& _description ) override;

		unsigned   m_id;
		static int s_type;
	};
}
