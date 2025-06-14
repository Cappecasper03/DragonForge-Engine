#pragma once

#include <string>

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/cTransform.h"
#include "engine/core/utils/Misc.h"
#include "engine/graphics/callback/iRenderCallback.h"
#include "iObject.h"

namespace df
{
	class iAsset : public iObject
	{
	public:
		DF_DeleteCopyAndMove( iAsset );

		iAsset( const std::string& _name );
		~iAsset() override = default;

		void         update( float /*_delta_time*/ = 0 ) override;
		virtual void render() = 0;

		cTransform                 m_transform;
		cShared< iRenderCallback > m_render_callback;
	};
}
