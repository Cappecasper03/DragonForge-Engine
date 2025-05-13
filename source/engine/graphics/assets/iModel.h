#pragma once

#include <assimp/postprocess.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/core/utils/Misc.h"
#include "iAsset.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df
{
	class iMesh;
	class iTexture;

	class iModel : public iAsset
	{
	public:
		DF_DisableCopyAndMove( iModel );

		explicit iModel( const std::string& _name );
		~iModel() override;

		void render() override;

		bool load( const std::string& _file_path, unsigned _load_flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace );

		std::vector< iMesh* >                        m_meshes;
		std::string                                  m_path;
		std::unordered_map< std::string, iTexture* > m_textures;

	protected:
		virtual bool processNode( const aiNode* _node, const aiScene* _scene ) = 0;
	};
}
