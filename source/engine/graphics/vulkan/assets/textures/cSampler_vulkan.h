#pragma once

#include <unordered_map>
#include <vulkan/vulkan.hpp>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/textures/iSampler.h"
#include "engine/graphics/types/sSamplerParameter.h"

namespace df::vulkan
{
	class cSampler_vulkan : public iSampler
	{
	public:
		DF_DeleteCopyAndMove( cSampler_vulkan );

		cSampler_vulkan();
		~cSampler_vulkan() override = default;

		void addParameter( const sSamplerParameter::eName _name, const sSamplerParameter::eParameter _parameter ) override { m_parameters[ _name ] = _parameter; }
		void removeParameter( const sSamplerParameter::eName _name ) override { m_parameters.erase( _name ); }

		void update() override;

		const vk::Sampler& get() const { return m_sampler.get(); }

	private:
		vk::UniqueSampler                                                             m_sampler;
		std::unordered_map< sSamplerParameter::eName, sSamplerParameter::eParameter > m_parameters;
	};
}
