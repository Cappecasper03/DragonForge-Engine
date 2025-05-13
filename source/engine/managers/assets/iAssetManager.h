#pragma once

#include "engine/graphics/assets/iAsset.h"
#include "engine/managers/iObjectManager.h"

namespace df
{
	struct iRenderCallback;

	template< typename T, typename Tasset >
	class iAssetManager : public iObjectManager< T, Tasset >
	{
	public:
		DF_DisableCopyAndMove( iAssetManager );

		iAssetManager();

		static void render();

		static iRenderCallback* getDefaultRenderCallback() { return iAssetManager::getInstance()->m_default_render_callback; }
		static iRenderCallback* getForcedRenderCallback() { return iAssetManager::getInstance()->m_forced_render_callback; }

	protected:
		iRenderCallback* m_default_render_callback;
		iRenderCallback* m_forced_render_callback;
	};
}

#include "iAssetManager.inl"
