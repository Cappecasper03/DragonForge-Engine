﻿#pragma once

#include "engine/graphics/assets/iQuad.h"
#include "iAssetManager.h"

namespace df
{
	class cQuadManager final : public iAssetManager< cQuadManager, iQuad >
	{
	public:
		DF_DeleteCopyAndMove( cQuadManager );

		cQuadManager();
		~cQuadManager() override;

		static iQuad* load( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white );
	};
}
