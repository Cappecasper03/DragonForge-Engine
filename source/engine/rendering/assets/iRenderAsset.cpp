#include "iRenderAsset.h"

#include <utility>

#include "engine/misc/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iRenderAsset::iRenderAsset( std::string _name )
		: iAsset( std::move( _name ) )
		, transform( new cTransform() )
	{}

	iRenderAsset::~iRenderAsset()
	{
		DF_ProfilingScopeCpu;

		delete transform;
	}

	void iRenderAsset::update( float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		transform->update();
	}
}
