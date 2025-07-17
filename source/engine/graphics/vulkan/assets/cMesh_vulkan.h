#pragma once

#include "cModel_vulkan.h"
#include "engine/graphics/assets/iMesh.h"
#include "sRenderAsset_vulkan.h"

struct aiScene;
struct aiMesh;

namespace df::vulkan
{
	class cMesh_vulkan : public sRenderAsset_vulkan,
						 public iMesh
	{
		friend cModel_vulkan;

	public:
		DF_DeleteCopyAndMove( cMesh_vulkan );

		cMesh_vulkan( const aiMesh* _mesh, const aiScene* _scene, cModel_vulkan* _parent );

		void render() override;

		const std::vector< vk::DescriptorSet >& getDescriptors() const { return m_descriptors; }
		static vk::DescriptorSetLayout          getLayout() { return s_descriptor_layout.get(); }

	private:
		void createTextures( const aiMesh* _mesh, const aiScene* _scene ) override;

		std::vector< vk::DescriptorSet >     m_descriptors;
		static vk::UniqueDescriptorSetLayout s_descriptor_layout;
	};
}
