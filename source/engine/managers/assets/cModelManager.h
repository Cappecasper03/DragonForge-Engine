#pragma once

#include "engine/rendering/assets/iModel.h"
#include "iAssetManager.h"

namespace df
{
	class cModelManager final : public iAssetManager< cModelManager, iModel >
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cModelManager )

		cModelManager();
		~cModelManager() override = default;

		static iModel* create( const std::string& _name );
	};
}
