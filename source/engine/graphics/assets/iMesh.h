#pragma once

#include <assimp/material.h>
#include <unordered_map>
#include <vector>

#include "engine/core/math/cVector.h"
#include "iRenderAsset.h"

struct aiScene;
struct aiMesh;

namespace df
{
	class iTexture;
	class iModel;

	class iMesh : public iRenderAsset
	{
	public:
		DF_DisableCopyAndMove( iMesh );

		struct sVertex
		{
			cVector3f position  = cVector3f( 0 );
			cVector3f normal    = cVector3f( 0 );
			cVector3f tangent   = cVector3f( 0 );
			cVector3f bitangent = cVector3f( 0 );
			cVector2f tex_coords;
		};

		explicit iMesh( const aiMesh* _mesh, const aiScene* _scene, iModel* _parent );
		~iMesh() override = default;

		const std::vector< sVertex >&                         getVertices() const { return m_vertices; }
		const std::vector< unsigned >&                        getIndices() const { return m_indices; }
		const std::unordered_map< aiTextureType, iTexture* >& getTextures() const { return m_textures; }

	protected:
		void         createVertices( const aiMesh* _mesh );
		void         createIndices( const aiMesh* _mesh );
		virtual void createTextures( const aiMesh* _mesh, const aiScene* _scene ) = 0;

		std::vector< sVertex >                         m_vertices;
		std::vector< unsigned >                        m_indices;
		std::unordered_map< aiTextureType, iTexture* > m_textures;

		iModel* m_parent;
	};
}
