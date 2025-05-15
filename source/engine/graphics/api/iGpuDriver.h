#pragma once

#include <Ultralight/platform/GPUDriver.h>
#include <vector>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iGpuDriver : public ultralight::GPUDriver
	{
	public:
		DF_DeleteCopyAndMove( iGpuDriver );

		iGpuDriver()           = default;
		~iGpuDriver() override = default;

		virtual void drawCommandList();

		bool hasPendingCommands() const { return !m_command_list.empty(); }

		void BeginSynchronize() override {}
		void EndSynchronize() override {}

		uint32_t NextTextureId() override { return m_next_texture_id++; }
		uint32_t NextRenderBufferId() override { return m_next_render_buffer_id++; }
		uint32_t NextGeometryId() override { return m_next_geometry_id++; }

		void UpdateCommandList( const ultralight::CommandList& _list ) override;

	protected:
		virtual void clearRenderBuffer( uint32_t _render_buffer_id ) = 0;

		virtual void drawGeometry( uint32_t _geometry_id, uint32_t _indices_count, uint32_t _indices_offset, const ultralight::GPUState& _state ) = 0;

		uint32_t                           m_next_texture_id       = 0;
		uint32_t                           m_next_render_buffer_id = 1;
		uint32_t                           m_next_geometry_id      = 0;
		std::vector< ultralight::Command > m_command_list;
	};
}
