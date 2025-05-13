#include "iModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <ranges>
#include <utility>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/profiling/ProfilingMacros.h"
#include "iMesh.h"
#include "iTexture.h"

namespace df
{
	iModel::iModel( std::string _name )
		: iRenderAsset( std::move( _name ) )
	{}

	iModel::~iModel()
	{
		DF_ProfilingScopeCpu;

		for( const iTexture* texture: m_textures | std::views::values )
			delete texture;

		for( const iMesh* mesh: m_meshes )
			delete mesh;
	}

	void iModel::render()
	{
		DF_ProfilingScopeCpu;

		for( iMesh* mesh: m_meshes )
		{
			if( !mesh->m_render_callback )
				mesh->m_render_callback = m_render_callback;

			mesh->render();

			if( mesh->m_render_callback == m_render_callback )
				mesh->m_render_callback = nullptr;
		}
	}

	bool iModel::load( const std::string& _file_path, const unsigned _load_flags )
	{
		DF_ProfilingScopeCpu;

		Assimp::Importer importer;

		{
			m_path               = cFileSystem::getPath( _file_path );
			const aiScene* scene = importer.ReadFile( m_path, _load_flags );

			if( scene && scene->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode )
				return processNode( scene->mRootNode, scene );
		}

		std::string extensions;
		importer.GetExtensionList( extensions );

		std::vector< std::string > extension_list;
		std::string                extension_temp;
		for( const char& character: extensions )
		{
			if( character == '*' )
				continue;

			if( character == ';' )
			{
				extension_list.push_back( extension_temp );
				extension_temp.clear();
				continue;
			}

			extension_temp += character;
		}

		for( const std::string& extension: extension_list )
		{
			m_path = cFileSystem::getPath( _file_path + extension );

			const aiScene* scene = importer.ReadFile( m_path, _load_flags );

			if( scene && scene->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode )
				return processNode( scene->mRootNode, scene );
		}

		DF_LogError( importer.GetErrorString() );
		return false;
	}
}
