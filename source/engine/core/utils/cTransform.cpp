﻿#include "cTransform.h"

#include "engine/core/Log.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cTransform::cTransform()
		: m_parent( nullptr )
	{}

	cTransform::~cTransform()
	{
		DF_ProfilingScopeCpu;

		if( m_parent )
			removeParent();

		while( !m_children.empty() )
			removeChild( *m_children.front() );
	}

	void cTransform::update()
	{
		DF_ProfilingScopeCpu;

		m_world = m_parent ? m_local * m_parent->m_world : m_local;

		for( cTransform* child: m_children )
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

		if( _child.m_parent )
		{
			DF_LogError( "Child already have a parent" );
			return false;
		}

		m_children.push_back( &_child );
		_child.m_parent = this;
		return true;
	}

	bool cTransform::removeChild( cTransform& _child )
	{
		DF_ProfilingScopeCpu;

		if( std::erase( m_children, &_child ) )
		{
			_child.m_parent = nullptr;
			return true;
		}

		DF_LogWarning( "Child doesn't exist" );
		return false;
	}

	bool cTransform::removeParent()
	{
		DF_ProfilingScopeCpu;

		if( !m_parent )
		{
			DF_LogWarning( "Parent doesn't exist" );
			return false;
		}

		m_parent->removeChild( *this );
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

		if( this->m_parent )
		{
			DF_LogWarning( "Already have a parent" );
			return false;
		}

		m_parent = &_parent;
		_parent.m_children.push_back( this );
		return true;
	}
}
