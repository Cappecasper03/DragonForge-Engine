#pragma once

#include <string>

#include "engine/misc/Misc.h"
#include "engine/rendering/assets/iModel.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df::vulkan
{
	class cMesh_vulkan;
	class cTexture_vulkan;

	class cModel_vulkan : public iModel
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cModel_vulkan )

		friend cMesh_vulkan;

		explicit cModel_vulkan( std::string _name );

	private:
		bool processNode( const aiNode* _node, const aiScene* _scene ) override;
	};
}
