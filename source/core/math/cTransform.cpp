#include "cTransform.h"

#include <algorithm>

#include "core/log/Log.h"

namespace df
{
    cTransform::cTransform()
    : m_parent( nullptr )
    {}

    cTransform::~cTransform()
    {
        if( m_parent )
            removeParent();

        while( !m_children.empty() )
            removeChild( *m_children.front() );
    }

    void cTransform::update()
    {
        m_render_matrix = m_parent ? matrix * m_parent->m_render_matrix : matrix;

        for( cTransform* child : m_children )
            child->update();
    }

    bool cTransform::addChild( cTransform& _child )
    {
        if( this == &_child )
        {
            LOG_ERROR( "Child can't be itself" );
            return false;
        }

        if( _child.m_parent )
        {
            LOG_ERROR( "Child already have a parent" );
            return false;
        }

        m_children.push_back( &_child );
        _child.m_parent = this;
        return true;
    }

    bool cTransform::removeChild( cTransform& _child )
    {
        if( std::erase( m_children, &_child ) )
        {
            _child.m_parent = nullptr;
            return true;
        }

        LOG_WARNING( "Child doesn't exist" );
        return false;
    }

    bool cTransform::removeParent()
    {
        if( !m_parent )
        {
            LOG_WARNING( "Parent doesn't exist" );
            return false;
        }

        m_parent->removeChild( *this );
        return true;
    }

    bool cTransform::setParent( cTransform& _parent )
    {
        if( this == &_parent )
        {
            LOG_WARNING( "Parent can't be itself" );
            return false;
        }

        if( this->m_parent )
        {
            LOG_WARNING( "Already have a parent" );
            return false;
        }

        m_parent = &_parent;
        _parent.m_children.push_back( this );
        return true;
    }
}
