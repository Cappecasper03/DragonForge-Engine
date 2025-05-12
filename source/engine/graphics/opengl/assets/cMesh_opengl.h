#pragma once

#include "engine/graphics/assets/iMesh.h"
#include "sRenderAsset_opengl.h"

struct aiScene;
struct aiMesh;

namespace df::opengl
{
	class cTexture_opengl;
	class cModel_opengl;

	class cMesh_opengl : public sRenderAsset_opengl,
						 public iMesh
	{
	public:
		DF_DisableCopyAndMove( cMesh_opengl );

		explicit cMesh_opengl( const aiMesh* _mesh, const aiScene* _scene, cModel_opengl* _parent );
		~cMesh_opengl() override = default;

		void render() override;

		cBuffer_opengl m_push_constant;

	private:
		void createTextures( const aiMesh* _mesh, const aiScene* _scene ) override;
	};
}
