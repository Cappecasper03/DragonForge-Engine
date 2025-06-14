#pragma once

namespace df
{
	template< typename T, typename Tasset >
	iAssetManager< T, Tasset >::iAssetManager()
		: m_default_render_callback( nullptr )
		, m_forced_render_callback( nullptr )
	{}

	template< typename T, typename Tasset >
	void iAssetManager< T, Tasset >::render()
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, cUnique< iObject > >& assets = iAssetManager::getInstance()->m_objects;

		for( cUnique< iObject >& asset: assets | std::views::values )
			reinterpret_cast< iAsset* >( asset.get() )->render();
	}
}
