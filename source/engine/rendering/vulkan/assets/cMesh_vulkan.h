#pragma once

#include "engine/rendering/assets/iMesh.h"
#include "sRenderAsset_vulkan.h"

struct aiScene;
struct aiMesh;

namespace df::vulkan
{
	class cTexture_vulkan;
	class cModel_vulkan;

	class cMesh_vulkan : public sRenderAsset_vulkan,
						 public iMesh
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cMesh_vulkan )

		explicit cMesh_vulkan( const aiMesh* _mesh, const aiScene* _scene, cModel_vulkan* _parent );
		~cMesh_vulkan() override = default;

		void render() override;

	private:
		void createTextures( const aiMesh* _mesh, const aiScene* _scene ) override;
	};
}
