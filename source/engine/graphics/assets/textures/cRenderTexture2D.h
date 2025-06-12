#pragma once
#include <string>

#include "engine/core/math/cVector.h"
#include "engine/graphics/types/sTextureFormat.h"
#include "engine/graphics/types/sTextureUsage.h"
#include "iTexture.h"

namespace df
{
	class cRenderTexture2D : public iTexture
	{
	public:
		DF_DeleteCopyAndMove( cRenderTexture2D );

		struct sDescription
		{
			std::string                name;
			cVector2u                  size       = cVector2u( 1 );
			unsigned                   mip_levels = 1;
			sTextureFormat::eFormat    format     = sTextureFormat::kRGBA;
			sTextureUsage::eUsageFlags usage      = sTextureUsage::kTransferSource | sTextureUsage::kTransferDestination | sTextureUsage::kStorage | sTextureUsage::kSampled
			                                 | sTextureUsage::kColorAttachment;
		};

		cRenderTexture2D()           = default;
		~cRenderTexture2D() override = default;

		const cVector2u&               getSize() const override { return m_description.size; }
		const sTextureFormat::eFormat& getFormat() const override { return m_description.format; }

		static cRenderTexture2D* create( const sDescription& _description );

	protected:
		virtual void initialize( const sDescription& _description ) { m_description = _description; }

		sDescription m_description;
	};
}
