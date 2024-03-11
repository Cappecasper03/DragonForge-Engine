#include "cModel_vulkan.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <ranges>
#include <tracy/Tracy.hpp>

#include "cMesh_vulkan.h"
#include "engine/filesystem/cFileSystem.h"

namespace df::vulkan
{
	cModel_vulkan::cModel_vulkan( std::string _name )
		: iModel( std::move( _name ) )
	{}

	bool cModel_vulkan::processNode( const aiNode* _node, const aiScene* _scene )
	{
		ZoneScoped;

		if( !_node )
			return false;

		for( unsigned i = 0; i < _node->mNumMeshes; ++i )
			meshes.push_back( new cMesh_vulkan( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

		for( unsigned i = 0; i < _node->mNumChildren; ++i )
			processNode( _node->mChildren[ i ], _scene );

		return true;
	}
}
