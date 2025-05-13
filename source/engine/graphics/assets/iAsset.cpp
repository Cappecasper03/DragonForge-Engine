#include "iAsset.h"

#include <utility>

#include "engine/core/utils/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iAsset::iAsset( std::string _name )
		: iObject( std::move( _name ) )
	{}

	void iAsset::update( float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		m_transform.update();
	}
}
