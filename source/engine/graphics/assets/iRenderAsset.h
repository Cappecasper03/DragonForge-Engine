#pragma once

#include <string>

#include "engine/core/utils/Misc.h"
#include "iAsset.h"

namespace df
{
	struct iRenderCallback;

	class cTransform;

	class iRenderAsset : public iAsset
	{
	public:
		DF_DisableCopyAndMove( iRenderAsset );

		explicit iRenderAsset( std::string _name );
		~iRenderAsset() override;

		void update( float /*_delta_time*/ = 0 ) override;
		void render() override = 0;

		cTransform*      m_transform;
		iRenderCallback* m_render_callback = nullptr;
	};
}
