﻿#pragma once

#include <functional>

// #include "engine/profiling/ProfilingMacros.h"
#include "engine/misc/Misc.h"
#include "vulkan/pipeline/cPipeline_vulkan.h"

namespace df
{
	struct iRenderCallback
	{
		DF_DisableCopyAndMove( iRenderCallback );

		explicit iRenderCallback( std::string _name )
			: name( std::move( _name ) )
		{}

		virtual ~iRenderCallback() = default;

		const std::string name;
	};

	template< typename T, typename... Targs >
	class cRenderCallback final : public iRenderCallback
	{
	public:
		DF_DisableCopyAndMove( cRenderCallback );

		explicit cRenderCallback( std::string _name, const std::string& _shader_name, void( _callback )( const T*, Targs... ) );
		explicit cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void( _callback )( const T*, Targs... ) );

		explicit cRenderCallback( std::string _name, const vulkan::sPipelineCreateInfo_vulkan& _pipeline, void( _callback )( const T*, Targs... ) );
		explicit cRenderCallback( std::string _name, const std::vector< vulkan::sPipelineCreateInfo_vulkan >& _pipelines, void( _callback )( const T*, Targs... ) );

		~cRenderCallback() override;

		void render( Targs... _args );

	protected:
		std::vector< T* >                           m_data;
		std::function< void( const T*, Targs... ) > m_callback;
	};

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::string& _shader_name, void( _callback )( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		// DF_ProfilingScopeCPU;

		m_data.push_back( new T( _shader_name ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void _callback( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		// DF_ProfilingScopeCPU;

		for( const std::string& shader_name: _shader_names )
			m_data.push_back( new T( shader_name ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const vulkan::sPipelineCreateInfo_vulkan& _pipeline, void _callback( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		// DF_ProfilingScopeCPU;

		m_data.push_back( new T( _pipeline ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::vector< vulkan::sPipelineCreateInfo_vulkan >& _pipelines, void _callback( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		// DF_ProfilingScopeCPU;

		for( const vulkan::sPipelineCreateInfo_vulkan& pipeline: _pipelines )
			m_data.push_back( new T( pipeline ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::~cRenderCallback()
	{
		// DF_ProfilingScopeCPU;

		for( const T* data: m_data )
			delete data;
	}

	template< typename T, typename... Targs >
	void cRenderCallback< T, Targs... >::render( Targs... _args )
	{
		// DF_ProfilingScopeCPU;

		for( T* data: m_data )
			m_callback( data, _args... );
	}
}
