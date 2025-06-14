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
	class cTexture2D;

	class iModel : public iAsset
	{
	public:
		DF_DeleteCopyAndMove( iModel );

		iModel( const std::string& _name );
		~iModel() override = default;

		void render() override;

		bool load( const std::string& _file_path, unsigned _load_flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace );

		std::vector< cUnique< iMesh > >                          m_meshes;
		std::string                                              m_path;
		std::unordered_map< std::string, cShared< cTexture2D > > m_textures;

	protected:
		virtual bool processNode( const aiNode* _node, const aiScene* _scene ) = 0;
	};
}
