#pragma once

#include <unordered_map>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/iSampler.h"
#include "engine/graphics/types/sSamplerParameter.h"

namespace df::opengl
{
	class cSampler_opengl : public iSampler
	{
	public:
		DF_DeleteCopyAndMove( cSampler_opengl );

		cSampler_opengl();
		~cSampler_opengl() override;

		void addParameter( const sSamplerParameter::eName _name, const sSamplerParameter::eParameter _parameter ) override { m_parameters[ _name ] = _parameter; }
		void removeParameter( const sSamplerParameter::eName _name ) override { m_parameters.erase( _name ); }

		void update() override;

		void bind( unsigned _index = 0 ) override;

	private:
		unsigned                                                                      m_id;
		std::unordered_map< sSamplerParameter::eName, sSamplerParameter::eParameter > m_parameters;
	};
}
