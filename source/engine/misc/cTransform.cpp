#include "cTransform.h"

#include "engine/log/Log.h"

#ifdef PROFILING
#include "engine/profiling/Profiling.h"
#endif

namespace df
{
    cTransform::cTransform()
    : local( 1 ),
      world( 1 ),
      parent( nullptr )
    {}

    cTransform::~cTransform()
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( parent )
            removeParent();

        while( !children.empty() )
            removeChild( *children.front() );
    }

    void cTransform::update()
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        world = parent ? local * parent->world : local;

        for( cTransform* child : children )
            child->update();
    }

    bool cTransform::addChild( cTransform& _child )
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( this == &_child )
        {
            LOG_ERROR( "Child can't be itself" );
            return false;
        }

        if( _child.parent )
        {
            LOG_ERROR( "Child already have a parent" );
            return false;
        }

        children.push_back( &_child );
        _child.parent = this;
        return true;
    }

    bool cTransform::removeChild( cTransform& _child )
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( std::erase( children, &_child ) )
        {
            _child.parent = nullptr;
            return true;
        }

        LOG_WARNING( "Child doesn't exist" );
        return false;
    }

    bool cTransform::removeParent()
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( !parent )
        {
            LOG_WARNING( "Parent doesn't exist" );
            return false;
        }

        parent->removeChild( *this );
        return true;
    }

    bool cTransform::setParent( cTransform& _parent )
    {
#ifdef PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( this == &_parent )
        {
            LOG_WARNING( "Parent can't be itself" );
            return false;
        }

        if( this->parent )
        {
            LOG_WARNING( "Already have a parent" );
            return false;
        }

        parent = &_parent;
        _parent.children.push_back( this );
        return true;
    }
}
