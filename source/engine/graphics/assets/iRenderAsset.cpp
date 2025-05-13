#include "iRenderAsset.h"

#include <utility>

#include "engine/core/utils/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iRenderAsset::iRenderAsset( std::string _name )
		: iAsset( std::move( _name ) )
		, m_transform( new cTransform() )
	{}

	iRenderAsset::~iRenderAsset()
	{
		DF_ProfilingScopeCpu;

		delete m_transform;
	}

	void iRenderAsset::update( float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		m_transform->update();
	}
}
