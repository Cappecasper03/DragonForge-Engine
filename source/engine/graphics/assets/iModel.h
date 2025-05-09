﻿#pragma once

#include <assimp/postprocess.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/misc/Misc.h"
#include "iRenderAsset.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df
{
	class iMesh;
	class iTexture;

	class iModel : public iRenderAsset
	{
	public:
		DF_DisableCopyAndMove( iModel );

		explicit iModel( std::string _name );
		~iModel() override;

		void render() override;

		bool load( const std::string& _folder_path, unsigned _load_flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices );

		std::vector< iMesh* >                        meshes;
		std::string                                  folder;
		std::unordered_map< std::string, iTexture* > textures;

	protected:
		virtual bool processNode( const aiNode* _node, const aiScene* _scene ) = 0;
	};
}
