#pragma once

#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>

namespace df
{
	// public
	template< int I, typename T >
	cVector< I, T >::cVector()
		: m_data( 0 )
	{}

	template< int I, typename T >
	cVector< I, T >::cVector( T _scalar )
		: m_data( _scalar )
	{}

	template< int I, typename T >
	cVector< I, T >::cVector( const T& _x, const T& _y )
		requires( I == 2 )
		: m_data( _x, _y )
	{}

	template< int I, typename T >
	cVector< I, T >::cVector( const T& _x, const T& _y, const T& _z )
		requires( I == 3 )
		: m_data( _x, _y, _z )
	{}

	template< int I, typename T >
	cVector< I, T >::cVector( const T& _x, const T& _y, const T& _z, const T& _w )
		requires( I == 4 )
		: m_data( _x, _y, _z, _w )
	{}

	template< int I, typename T >
	template< int J >
	cVector< I, T >::cVector( const cVector< J, T >& _vector )
		requires( J == I + 1 || J == I || J == I - 1 )
		: m_data( _vector.m_data )
	{}

	template< int I, typename T >
	template< int J >
	cVector< I, T >::cVector( const cVector< J, T >&& _vector )
		requires( J == I + 1 || J == I || J == I - 1 )
		: m_data( std::move( _vector.m_data ) )
	{}

	template< int I, typename T >
	template< int J >
	cVector< I, T >::cVector( const cVector< J, T >& _vector, const T& _value )
		requires( J == I - 1 )
		: m_data( _vector.m_data, _value )
	{}

	template< int I, typename T >
	template< int J, typename R >
	cVector< I, T >::cVector( const cVector< J, R >& _vector )
		requires( J == I + 1 || J == I || J == I - 1 )
		: m_data( _vector.m_data )
	{}

	template< int I, typename T >
	template< int J, typename R >
	cVector< I, T >::cVector( const cVector< J, R >&& _vector )
		requires( J == I + 1 || J == I || J == I - 1 )
		: m_data( std::move( _vector.m_data ) )
	{}

	template< int I, typename T >
	template< int J, typename R >
	cVector< I, T >::cVector( const cVector< J, R >& _vector, const T& _value )
		requires( J == I - 1 )
		: m_data( _vector.m_data, _value )
	{}

	template< int I, typename T >
	T& cVector< I, T >::x()
		requires( I >= 1 )
	{
		return m_data.x;
	}
	template< int I, typename T >
	const T& cVector< I, T >::x() const
		requires( I >= 1 )
	{
		return m_data.x;
	}

	template< int I, typename T >
	T& cVector< I, T >::y()
		requires( I >= 2 )
	{
		return m_data.y;
	}
	template< int I, typename T >
	const T& cVector< I, T >::y() const
		requires( I >= 2 )
	{
		return m_data.y;
	}

	template< int I, typename T >
	T& cVector< I, T >::z()
		requires( I >= 3 )
	{
		return m_data.z;
	}
	template< int I, typename T >
	const T& cVector< I, T >::z() const
		requires( I >= 3 )
	{
		return m_data.z;
	}

	template< int I, typename T >
	T& cVector< I, T >::w()
		requires( I >= 4 )
	{
		return m_data.w;
	}
	template< int I, typename T >
	const T& cVector< I, T >::w() const
		requires( I >= 4 )
	{
		return m_data.w;
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator+=( const cVector& _vector )
	{
		m_data += _vector.m_data;
		return *this;
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator-=( const cVector& _vector )
	{
		m_data -= _vector.m_data;
		return *this;
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator*=( const cVector& _vector )
	{
		m_data *= _vector.m_data;
		return *this;
	}
	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator/=( const cVector& _vector )
	{
		m_data /= _vector.m_data;
		return *this;
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator+=( T _scalar )
	{
		m_data += _scalar;
		return *this;
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator-=( T _scalar )
	{
		m_data -= _scalar;
		return *this;
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator*=( T _scalar )
	{
		m_data *= _scalar;
		return *this;
	}
	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::operator/=( T _scalar )
	{
		m_data /= _scalar;
		return *this;
	}

	template< int I, typename T >
	cVector< I, T > cVector< I, T >::normalized() const
	{
		return glm::normalize( m_data );
	}

	template< int I, typename T >
	cVector< I, T >& cVector< I, T >::normalize()
	{
		glm::normalize( m_data );
		return *this;
	}

	// Private
	template< int I, typename T >
	cVector< I, T >::cVector( const glm::vec< I, T >& _vector )
		: m_data( _vector )
	{}

	template< int I, typename T >
	cVector< I, T >::cVector( glm::vec< I, T >&& _vector )
		: m_data( std::move( _vector ) )
	{}
}