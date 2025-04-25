#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace df
{
	template< int L, typename T >
	cMatrix< L, T >::cMatrix()
		: m_data( 1 )
	{}

	template< int L, typename T >
	cMatrix< L, T >::cMatrix( T _scalar )
		: m_data( _scalar )
	{}

	template< int L, typename T >
	cMatrix< L, T >::cMatrix( const cVector< L, T >& _right, const cVector< L, T >& _up, const cVector< L, T >& _backward, const cVector< L, T >& _position )
		: m_data( _right.m_data, _up.m_data, _backward.m_data, _position.m_data )
	{}

	template< int L, typename T >
	cMatrixVectorProxy< 4, T > cMatrix< L, T >::right()
		requires( L == 4 )
	{
		return cMatrixVectorProxy< 4, T >( this, 0 );
	}
	template< int L, typename T >
	cMatrixVectorProxy< 4, T > cMatrix< L, T >::up()
		requires( L == 4 )
	{
		return cMatrixVectorProxy< 4, T >( this, 1 );
	}
	template< int L, typename T >
	cMatrixVectorProxy< 4, T > cMatrix< L, T >::backward()
		requires( L == 4 )
	{
		return cMatrixVectorProxy< 4, T >( this, 2 );
	}
	template< int L, typename T >
	cMatrixVectorProxy< 4, T > cMatrix< L, T >::position()
		requires( L == 4 )
	{
		return cMatrixVectorProxy< 4, T >( this, 3 );
	}

	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator+=( const cMatrix& _matrix )
	{
		m_data += _matrix.m_data;
		return *this;
	}
	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator-=( const cMatrix& _matrix )
	{
		m_data -= _matrix.m_data;
		return *this;
	}
	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator*=( const cMatrix& _matrix )
	{
		m_data *= _matrix.m_data;
		return *this;
	}
	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator/=( const cMatrix& _matrix )
	{
		m_data /= _matrix.m_data;
		return *this;
	}

	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator+=( T _scalar )
	{
		m_data += _scalar;
		return *this;
	}
	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator-=( T _scalar )
	{
		m_data -= _scalar;
		return *this;
	}
	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator*=( T _scalar )
	{
		m_data *= _scalar;
		return *this;
	}
	template< int L, typename T >
	cMatrix< L, T >& cMatrix< L, T >::operator/=( T _scalar )
	{
		m_data /= _scalar;
		return *this;
	}

	template< int L, typename T >
	T* cMatrix< L, T >::data()
	{
		return glm::value_ptr( m_data );
	}
	template< int L, typename T >
	T const* cMatrix< L, T >::data() const
	{
		return glm::value_ptr( m_data );
	}

	template< int L, typename T >
	void cMatrix< L, T >::translate( const cVector< L - 1, T >& _vector )
	{
		m_data = glm::translate( m_data, _vector.m_data );
	}
	template< int L, typename T >
	cMatrix< L, T > cMatrix< L, T >::translated( const cVector< L - 1, T >& _vector )
	{
		return glm::translate( m_data, _vector.m_data );
	}

	template< int L, typename T >
	void cMatrix< L, T >::rotate( T _radians, const cVector< L - 1, T >& _vector )
	{
		m_data = glm::rotate( m_data, _radians, _vector.m_data );
	}
	template< int L, typename T >
	cMatrix< L, T > cMatrix< L, T >::rotated( T _radians, const cVector< L - 1, T >& _vector )
	{
		return glm::rotate( m_data, _radians, _vector.m_data );
	}

	template< int L, typename T >
	void cMatrix< L, T >::inverse()
	{
		m_data = glm::inverse( m_data );
	}
	template< int L, typename T >
	cMatrix< L, T > cMatrix< L, T >::inversed()
	{
		return glm::inverse( m_data );
	}

	template< int L, typename T >
	cMatrix< L, T > cMatrix< L, T >::fromPerspective( T _radians, T _aspect_ratio, T _near_clip, T _far_clip )
	{
		return glm::perspective( _radians, _aspect_ratio, _near_clip, _far_clip );
	}

	template< int L, typename T >
	cMatrix< L, T > cMatrix< L, T >::fromOrtho( T _left, T _right, T _bottom, T _top, T _near_clip, T _far_clip )
	{
		return glm::ortho( _left, _right, _bottom, _top, _near_clip, _far_clip );
	}

	template< int L, typename T >
	cMatrix< L, T >::cMatrix( const glm::mat< L, L, T >& _matrix )
		: m_data( _matrix )
	{}

	template< int L, typename T >
	cMatrix< L, T >::cMatrix( glm::mat< L, L, T >&& _matrix )
		: m_data( std::move( _matrix ) )
	{}
}