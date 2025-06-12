#include "cSampler_vulkan.h"

#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsDevice_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan
{
	cSampler_vulkan::cSampler_vulkan()
		: m_sampler( nullptr )
	{}

	void cSampler_vulkan::update()
	{
		DF_ProfilingScopeCpu;

		const cGraphicsDevice_vulkan* renderer = reinterpret_cast< cGraphicsDevice_vulkan* >( cRenderer::getGraphicsDevice() );

		vk::Filter mag = vk::Filter::eLinear;
		if( m_parameters.contains( sSamplerParameter::kMagFilter ) )
			mag = static_cast< vk::Filter >( sSamplerParameter::toVulkan( m_parameters.at( sSamplerParameter::kMagFilter ) ) );

		vk::Filter min = vk::Filter::eLinear;
		if( m_parameters.contains( sSamplerParameter::kMinFilter ) )
			min = static_cast< vk::Filter >( sSamplerParameter::toVulkan( m_parameters.at( sSamplerParameter::kMinFilter ) ) );

		vk::SamplerMipmapMode mipmap = vk::SamplerMipmapMode::eLinear;
		if( m_parameters.contains( sSamplerParameter::kMipMapFilter ) )
			mipmap = static_cast< vk::SamplerMipmapMode >( sSamplerParameter::toVulkan( m_parameters.at( sSamplerParameter::kMipMapFilter ) ) );

		vk::SamplerAddressMode wrap_s = vk::SamplerAddressMode::eRepeat;
		if( m_parameters.contains( sSamplerParameter::kWrapS ) )
			wrap_s = static_cast< vk::SamplerAddressMode >( sSamplerParameter::toVulkan( m_parameters.at( sSamplerParameter::kWrapS ) ) );

		vk::SamplerAddressMode wrap_t = vk::SamplerAddressMode::eRepeat;
		if( m_parameters.contains( sSamplerParameter::kWrapT ) )
			wrap_t = static_cast< vk::SamplerAddressMode >( sSamplerParameter::toVulkan( m_parameters.at( sSamplerParameter::kWrapT ) ) );

		m_sampler.release();
		m_sampler = renderer->getLogicalDevice().createSamplerUnique( vk::SamplerCreateInfo( vk::SamplerCreateFlags(), mag, min, mipmap, wrap_s, wrap_t ) ).value;
	}
}