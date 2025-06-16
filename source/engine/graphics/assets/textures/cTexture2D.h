#pragma once

#include <string>

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/types/sTextureUsage.h"
#include "iTexture.h"

namespace df
{
	class cTexture2D : public iTexture
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D );

		struct sDescription
		{
			std::string                name;
			cVector2u                  size       = cVector2u( 1 );
			unsigned                   mip_levels = 1;
			sTextureFormat::eFormat    format     = sTextureFormat::kRGBA;
			sTextureUsage::eUsageFlags usage      = sTextureUsage::kSampled | sTextureUsage::kTransferDestination;
		};

		struct sImageInfo
		{
			cVector2u               size;
			sTextureFormat::eFormat format;
		};

		cTexture2D()           = default;
		~cTexture2D() override = default;

		virtual void uploadData( const void* _data, sTextureFormat::eFormat _format, unsigned _mip_level = 0, bool _generate_mipmap = true ) {}
		bool         uploadDataFromFile( const std::string& _file, sTextureFormat::eFormat _format, unsigned _mip_level = 0, bool _generate_mipmap = true, bool _flip = true );

		const cVector2u&               getSize() const override { return m_description.size; }
		const sTextureFormat::eFormat& getFormat() const override { return m_description.format; }

		static cUnique< cTexture2D > create( const sDescription& _description );

		static sImageInfo getInfoFromFile( const std::string& _file );

	protected:
		virtual void initialize( const sDescription& _description ) { m_description = _description; }

		sDescription m_description;
	};
}
