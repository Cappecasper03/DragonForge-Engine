#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "engine/rendering/assets/iModel.h"
#include "sRenderAsset.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df::opengl
{
	class cMesh;
	class cTexture;

	class cModel : public sRenderAsset,
				   public iModel
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cModel )

		friend cMesh;

		explicit cModel( std::string _name );
		~cModel() override = default;

	private:
		bool processNode( const aiNode* _node, const aiScene* _scene ) override;
	};
}
