#pragma once

#include <string>

#include "engine/core/utils/Misc.h"
#include "iObject.h"

namespace df
{
	struct iRenderCallback;

	class cTransform;

	class iAsset : public iObject
	{
	public:
		DF_DisableCopyAndMove( iAsset );

		explicit iAsset( std::string _name );
		~iAsset() override;

		void         update( float /*_delta_time*/ = 0 ) override;
		virtual void render() = 0;

		cTransform*      m_transform;
		iRenderCallback* m_render_callback = nullptr;
	};
}
