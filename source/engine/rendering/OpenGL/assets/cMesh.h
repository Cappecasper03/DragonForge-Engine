﻿#pragma once

#include "engine/rendering/assets/iMesh.h"
#include "sRenderAsset.h"

struct aiScene;
struct aiMesh;

namespace df::opengl
{
	class cTexture;
	class cModel;

	class cMesh : public sRenderAsset,
				  public iMesh
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cMesh )

		explicit cMesh( const aiMesh* _mesh, const aiScene* _scene, cModel* _parent );
		~cMesh() override = default;

		void render() override;

	private:
		void createTextures( const aiMesh* _mesh, const aiScene* _scene ) override;
	};
}