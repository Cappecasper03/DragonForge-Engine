﻿#pragma once

#include "engine/graphics/assets/iModel.h"
#include "iAssetManager.h"

namespace df
{
	class cModelManager final : public iAssetManager< cModelManager, iModel >
	{
	public:
		DF_DeleteCopyAndMove( cModelManager );

		cModelManager();
		~cModelManager() override;

		static iModel* load( const std::string& _name, const std::string& _file_path, unsigned _load_flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace );
	};
}
