#pragma once

#include <string>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/iModel.h"
#include "sAsset_opengl.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df::opengl
{
	class cMesh_opengl;
	class cTexture2D_opengl;

	class cModel_opengl : public sAsset_opengl,
						  public iModel
	{
	public:
		DF_DeleteCopyAndMove( cModel_opengl );

		friend cMesh_opengl;

		explicit cModel_opengl( const std::string& _name );
		~cModel_opengl() override = default;

		static iRenderCallback* createDefaults();

	private:
		bool processNode( const aiNode* _node, const aiScene* _scene ) override;
	};
}
