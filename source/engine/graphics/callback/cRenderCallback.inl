﻿#pragma once

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::string& _shader_name, void( _callback )( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		DF_ProfilingScopeCpu;

		m_data.push_back( new T( _shader_name ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void _callback( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		DF_ProfilingScopeCpu;

		for( const std::string& shader_name: _shader_names )
			m_data.push_back( new T( shader_name ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const vulkan::cPipelineCreateInfo_vulkan& _pipeline, void _callback( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		DF_ProfilingScopeCpu;

		m_data.push_back( new T( _pipeline ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::vector< vulkan::cPipelineCreateInfo_vulkan >& _pipelines, void _callback( const T*, Targs... ) )
		: iRenderCallback( std::move( _name ) )
		, m_callback( _callback )
	{
		DF_ProfilingScopeCpu;

		for( const vulkan::cPipelineCreateInfo_vulkan& pipeline: _pipelines )
			m_data.push_back( new T( pipeline ) );
	}

	template< typename T, typename... Targs >
	cRenderCallback< T, Targs... >::~cRenderCallback()
	{
		DF_ProfilingScopeCpu;

		for( const T* data: m_data )
			delete data;
	}

	template< typename T, typename... Targs >
	void cRenderCallback< T, Targs... >::render( Targs... _args )
	{
		DF_ProfilingScopeCpu;

		for( T* data: m_data )
			m_callback( data, _args... );
	}
}
