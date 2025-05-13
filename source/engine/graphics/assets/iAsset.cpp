#include "iAsset.h"

#include "engine/core/utils/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	iAsset::iAsset( const std::string& _name )
		: iObject( _name )
	{}

	void iAsset::update( float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		m_transform.update();
	}
}
