#pragma once

#include <string>

#include "engine/misc/cTransform.h"
#include "engine/misc/Misc.h"

namespace df
{
	struct iRenderCallback;

	struct iAsset
	{
		DF_DISABLE_COPY_AND_MOVE( iAsset );

		explicit iAsset( std::string _name );
		virtual ~iAsset() = default;

		virtual void update( float /*_delta_time*/ = 0 ) {}
		virtual void render() {}

		const std::string name;
	};

	/*namespace vulkan
	{
	    struct sRendererSpecific : iRendererSpecific
	    {
	        DF_DISABLE_COPY_AND_MOVE( sRendererSpecific );

	        sRendererSpecific();
	        ~sRendererSpecific();

	        VkBuffer       vertex_buffer;
	        VkDeviceMemory vertex_buffer_memory;
	    };
	}*/

	class iRenderAsset : public iAsset
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( iRenderAsset );

		explicit iRenderAsset( std::string _name );
		~iRenderAsset() override;

		void update( float /*_delta_time*/ = 0 ) override { transform->update(); }
		void render() override = 0;

		cTransform*      transform;
		iRenderCallback* render_callback;
	};
}
