﻿#include "iModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <ranges>
#include <utility>

#include "core/cFileSystem.h"
#include "core/Log.h"
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

		for( const iTexture* texture: textures | std::views::values )
			delete texture;

		for( const iMesh* mesh: meshes )
			delete mesh;
	}

	void iModel::render()
	{
		DF_ProfilingScopeCpu;

		for( iMesh* mesh: meshes )
		{
			if( !mesh->render_callback )
				mesh->render_callback = render_callback;

			mesh->render();

			if( mesh->render_callback == render_callback )
				mesh->render_callback = nullptr;
		}
	}

	bool iModel::load( const std::string& _folder_path, const unsigned _load_flags )
	{
		DF_ProfilingScopeCpu;

		folder = cFileSystem::getPath( _folder_path );

		Assimp::Importer importer;

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
			const aiScene* scene = importer.ReadFile( folder + "/model" + extension, _load_flags );

			if( scene && scene->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE && scene->mRootNode )
				return processNode( scene->mRootNode, scene );
		}

		DF_LogError( importer.GetErrorString() );
		return false;
	}
}
