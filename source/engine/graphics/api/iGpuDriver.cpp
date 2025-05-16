#include "iGpuDriver.h"

#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	void iGpuDriver::drawCommandList()
	{
		DF_ProfilingScopeCpu;

		if( m_command_list.empty() )
			return;

		for( const ultralight::Command& command: m_command_list )
		{
			switch( command.command_type )
			{
				case ultralight::CommandType::ClearRenderBuffer: clearRenderBuffer( command.gpu_state.render_buffer_id );
				case ultralight::CommandType::DrawGeometry:      drawGeometry( command.geometry_id, command.indices_count, command.indices_offset, command.gpu_state );
			}
		}

		m_command_list.clear();
	}

	void iGpuDriver::UpdateCommandList( const ultralight::CommandList& _list )
	{
		DF_ProfilingScopeCpu;

		if( !_list.size )
			return;

		m_command_list.resize( _list.size );
		std::memcpy( m_command_list.data(), _list.commands, sizeof( ultralight::Command ) * _list.size );
	}
}
