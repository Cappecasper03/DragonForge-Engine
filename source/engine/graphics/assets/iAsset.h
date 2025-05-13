#pragma once

#include <string>

#include "engine/core/utils/cTransform.h"
#include "engine/core/utils/Misc.h"
#include "iObject.h"

namespace df
{
	struct iRenderCallback;

	class iAsset : public iObject
	{
	public:
		DF_DisableCopyAndMove( iAsset );

		explicit iAsset( const std::string& _name );
		~iAsset() override = default;

		void         update( float /*_delta_time*/ = 0 ) override;
		virtual void render() = 0;

		cTransform       m_transform;
		iRenderCallback* m_render_callback = nullptr;
	};
}
