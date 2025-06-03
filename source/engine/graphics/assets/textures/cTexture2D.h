#pragma once

#include <string>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/types/sTextureFormat.h"
#include "iTexture.h"

namespace df
{
	class cTexture2D : public iTexture
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D );

		struct sDescription
		{
			std::string             name;
			cVector2u               size       = cVector2u( 1 );
			unsigned                mip_levels = 0;
			sTextureFormat::eFormat format     = sTextureFormat::kRGB;
		};

		cTexture2D()           = default;
		~cTexture2D() override = default;

		virtual void uploadData( const void* _data, sTextureFormat::eFormat _format, unsigned _mip_level = 0, bool _generate_mipmap = true ) = 0;
		void         uploadDataFromFile( const std::string& _file, sTextureFormat::eFormat _format, unsigned _mip_level = 0, bool _generate_mipmap = true, bool _flip = true );

		const cVector2u& getSize() const override { return m_description.size; }

		static cTexture2D* create( const sDescription& _description );

	protected:
		virtual void initialize( const sDescription& _description ) { m_description = _description; }

		sDescription m_description;
	};
}
