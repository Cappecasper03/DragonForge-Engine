#include "cTransform.h"

#include "core/Log.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cTransform::cTransform()
		: parent( nullptr )
	{}

	cTransform::~cTransform()
	{
		DF_ProfilingScopeCpu;

		if( parent )
			removeParent();

		while( !children.empty() )
			removeChild( *children.front() );
	}

	void cTransform::update()
	{
		DF_ProfilingScopeCpu;

		world = parent ? local * parent->world : local;

		for( cTransform* child: children )
			child->update();
	}

	bool cTransform::addChild( cTransform& _child )
	{
		DF_ProfilingScopeCpu;

		if( this == &_child )
		{
			DF_LogError( "Child can't be itself" );
			return false;
		}

		if( _child.parent )
		{
			DF_LogError( "Child already have a parent" );
			return false;
		}

		children.push_back( &_child );
		_child.parent = this;
		return true;
	}

	bool cTransform::removeChild( cTransform& _child )
	{
		DF_ProfilingScopeCpu;

		if( std::erase( children, &_child ) )
		{
			_child.parent = nullptr;
			return true;
		}

		DF_LogWarning( "Child doesn't exist" );
		return false;
	}

	bool cTransform::removeParent()
	{
		DF_ProfilingScopeCpu;

		if( !parent )
		{
			DF_LogWarning( "Parent doesn't exist" );
			return false;
		}

		parent->removeChild( *this );
		return true;
	}

	bool cTransform::setParent( cTransform& _parent )
	{
		DF_ProfilingScopeCpu;

		if( this == &_parent )
		{
			DF_LogWarning( "Parent can't be itself" );
			return false;
		}

		if( this->parent )
		{
			DF_LogWarning( "Already have a parent" );
			return false;
		}

		parent = &_parent;
		_parent.children.push_back( this );
		return true;
	}
}
