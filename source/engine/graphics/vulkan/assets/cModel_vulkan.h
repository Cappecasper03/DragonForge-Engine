#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/assets/iModel.h"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace df::vulkan
{
	class cMesh_vulkan;
	class cTexture2D_vulkan;

	class cModel_vulkan : public iModel
	{
	public:
		DF_DeleteCopyAndMove( cModel_vulkan );

		friend cMesh_vulkan;

		explicit cModel_vulkan( const std::string& _name );
		~cModel_vulkan() override = default;

		static iRenderCallback* createDefaults();
		static void             destroyDefaults();

	private:
		bool processNode( const aiNode* _node, const aiScene* _scene ) override;

		static iRenderCallback* createDefaultsDeferred();
	};
}
