﻿#include "cModel_opengl.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <ranges>

#include "cMesh_opengl.h"
#include "engine/filesystem/cFileSystem.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/rendering/cRenderer.h"
#include "engine/rendering/opengl/callbacks/DefaultMeshCB_opengl.h"

namespace df::opengl
{
	cModel_opengl::cModel_opengl( std::string _name )
		: iModel( std::move( _name ) )
	{}

	iRenderCallback* cModel_opengl::createDefaults()
	{
		DF_ProfilingScopeCpu;

		iRenderCallback* callback;

		if( cRenderer::isDeferred() )
			callback = cRenderCallbackManager::create( "deferred_mesh", render_callback::deferredMesh );
		else
		{
			const std::vector< std::string > shader_names = { "forward_mesh_ambient" };
			callback                                      = cRenderCallbackManager::create( "forward_mesh", shader_names, render_callback::forwardMesh );
		}

		return callback;
	}

	bool cModel_opengl::processNode( const aiNode* _node, const aiScene* _scene )
	{
		DF_ProfilingScopeCpu;

		if( !_node )
			return false;

		for( unsigned i = 0; i < _node->mNumMeshes; ++i )
			meshes.push_back( new cMesh_opengl( _scene->mMeshes[ _node->mMeshes[ i ] ], _scene, this ) );

		for( unsigned i = 0; i < _node->mNumChildren; ++i )
			processNode( _node->mChildren[ i ], _scene );

		return true;
	}
}
