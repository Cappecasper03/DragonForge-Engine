#pragma once

#include "cTexture2D.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cSmartPointers.h"
#include "engine/graphics/types/sTextureFormat.h"

namespace df
{
	class cRenderTexture2D : public cTexture2D
	{
	public:
		DF_DeleteCopyAndMove( cRenderTexture2D );
		cRenderTexture2D()           = default;
		~cRenderTexture2D() override = default;

		const cVector2u&               getSize() const override { return m_description.size; }
		const sTextureFormat::eFormat& getFormat() const override { return m_description.format; }

		static cUnique< cRenderTexture2D > create( const sDescription& _description );
	};
}
