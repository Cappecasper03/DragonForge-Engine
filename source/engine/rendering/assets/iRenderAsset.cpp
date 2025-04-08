#include "iRenderAsset.h"

#include <utility>

#include "engine/misc/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iRenderAsset::iRenderAsset( std::string _name )
		: iAsset( std::move( _name ) )
	{}

	iRenderAsset::~iRenderAsset()
	{
		DF_ProfilingScopeCPU;

		delete transform;
	}

	void iRenderAsset::update( float /*_delta_time*/ )
	{
		DF_ProfilingScopeCPU;

		transform->update();
	}
}
