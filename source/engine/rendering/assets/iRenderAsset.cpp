﻿#include "iRenderAsset.h"

#include "engine/misc/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
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
