#include "AssetTypes.h"

#include "engine/profiling/ProfilingMacros.h"
#include "engine/misc/cTransform.h"

namespace df
{
	iAsset::iAsset( std::string _name )
		: name( std::move( _name ) )
	{}

	iRenderAsset::iRenderAsset( std::string _name )
		: iAsset( std::move( _name ) )
		, transform( new cTransform() )
		, render_callback( nullptr )
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
