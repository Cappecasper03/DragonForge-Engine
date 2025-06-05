#pragma once

#include "engine/core/utils/Misc.h"
#include "engine/graphics/types/sSamplerParameter.h"

namespace df
{
	class iSampler
	{
	public:
		DF_DeleteCopyAndMove( iSampler );

		iSampler()          = default;
		virtual ~iSampler() = default;

		virtual void addParameter( sSamplerParameter::eName _name, sSamplerParameter::eParameter _parameter ) = 0;
		virtual void removeParameter( sSamplerParameter::eName _name )                                        = 0;

		virtual void update() = 0;

		static iSampler* create();
	};
}
