#pragma once

#include <string>
#include <unordered_map>

#include "engine/misc/iSingleton.h"
#include "engine/misc/Misc.h"

namespace df
{
	class iRenderCallback;
	class iAsset;

	template< typename T, typename Tasset >
	class iAssetManager : public iSingleton< T >
	{
	public:
		DF_DisableCopyAndMove( iAssetManager );

		iAssetManager();
		~iAssetManager() override;

		template< typename Ttype = Tasset, typename... Targs >
		static Tasset* create( const std::string& _name, Targs... _args );

		static bool add( Tasset* _asset );

		static void update( float _delta_time );
		static void render();

		static bool destroy( const std::string& _name );
		static bool destroy( const Tasset* _asset );
		static void clear();

		static Tasset* get( const std::string& _name );

		static iRenderCallback* getDefaultRenderCallback() { return iAssetManager::getInstance()->m_default_render_callback; }
		static iRenderCallback* getForcedRenderCallback() { return iAssetManager::getInstance()->m_forced_render_callback; }

	protected:
		std::unordered_map< std::string, iAsset* > m_assets;
		iRenderCallback*                           m_default_render_callback;
		iRenderCallback*                           m_forced_render_callback;
	};
}

#include "iAssetManager.inl"