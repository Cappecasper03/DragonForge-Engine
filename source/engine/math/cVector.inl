#pragma once

#include <glm/geometric.hpp>

namespace df
{
	template< int L, typename T >
	cVector< L, T >::cVector()
		: m_data( 0 )
	{}

	template< int L, typename T >
	cVector< L, T >::cVector( T _scalar )
		: m_data( _scalar )
	{}

	template< int L, typename T >
	cVector< L, T >::cVector( const T& _x, const T& _y )
		requires( L == 2 )
		: m_data( _x, _y )
	{}
	template< int L, typename T >
	cVector< L, T >::cVector( const T& _x, const T& _y, const T& _z )
		requires( L == 3 )
		: m_data( _x, _y, _z )
	{}
	template< int L, typename T >
	cVector< L, T >::cVector( const T& _x, const T& _y, const T& _z, const T& _w )
		requires( L == 4 )
		: m_data( _x, _y, _z, _w )
	{}

	template< int L, typename T >
	template< int L1 >
	cVector< L, T >::cVector( const cVector< L1, T >& _vector )
		requires( L1 == L + 1 || L1 == L || L1 == L - 1 )
		: m_data( _vector.m_data )
	{}

	template< int L, typename T >
	template< int L1 >
	cVector< L, T >::cVector( const cVector< L1, T >&& _vector )
		requires( L1 == L + 1 || L1 == L || L1 == L - 1 )
		: m_data( std::move( _vector.m_data ) )
	{}

	template< int L, typename T >
	template< int L1 >
	cVector< L, T >::cVector( const cVector< L1, T >& _vector, const T& _value )
		requires( L1 == L - 1 )
		: m_data( _vector.m_data, _value )
	{}

	template< int L, typename T >
	template< int L1, typename T1 >
	cVector< L, T >::cVector( const cVector< L1, T1 >& _vector )
		requires( L1 == L + 1 || L1 == L || L1 == L - 1 )
		: m_data( _vector.m_data )
	{}

	template< int L, typename T >
	template< int L1, typename T1 >
	cVector< L, T >::cVector( const cVector< L1, T1 >&& _vector )
		requires( L1 == L + 1 || L1 == L || L1 == L - 1 )
		: m_data( std::move( _vector.m_data ) )
	{}

	template< int L, typename T >
	template< int L1, typename T1 >
	cVector< L, T >::cVector( const cVector< L1, T1 >& _vector, const T& _value )
		requires( L1 == L - 1 )
		: m_data( _vector.m_data, _value )
	{}

	template< int L, typename T >
	T& cVector< L, T >::operator[]( const int _index )
	{
		return m_data[ _index ];
	}
	template< int L, typename T >
	const T& cVector< L, T >::operator[]( const int _index ) const
	{
		return m_data[ _index ];
	}

	template< int L, typename T >
	T& cVector< L, T >::x()
		requires( L >= 1 )
	{
		return m_data.x;
	}
	template< int L, typename T >
	const T& cVector< L, T >::x() const
		requires( L >= 1 )
	{
		return m_data.x;
	}

	template< int L, typename T >
	T& cVector< L, T >::y()
		requires( L >= 2 )
	{
		return m_data.y;
	}
	template< int L, typename T >
	const T& cVector< L, T >::y() const
		requires( L >= 2 )
	{
		return m_data.y;
	}

	template< int L, typename T >
	T& cVector< L, T >::z()
		requires( L >= 3 )
	{
		return m_data.z;
	}
	template< int L, typename T >
	const T& cVector< L, T >::z() const
		requires( L >= 3 )
	{
		return m_data.z;
	}

	template< int L, typename T >
	T& cVector< L, T >::w()
		requires( L >= 4 )
	{
		return m_data.w;
	}
	template< int L, typename T >
	const T& cVector< L, T >::w() const
		requires( L >= 4 )
	{
		return m_data.w;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator+=( const cVector& _vector )
	{
		m_data += _vector.m_data;
		return *this;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator-=( const cVector& _vector )
	{
		m_data -= _vector.m_data;
		return *this;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator*=( const cVector& _vector )
	{
		m_data *= _vector.m_data;
		return *this;
	}
	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator/=( const cVector& _vector )
	{
		m_data /= _vector.m_data;
		return *this;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator+=( T _scalar )
	{
		m_data += _scalar;
		return *this;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator-=( T _scalar )
	{
		m_data -= _scalar;
		return *this;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator*=( T _scalar )
	{
		m_data *= _scalar;
		return *this;
	}
	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::operator/=( T _scalar )
	{
		m_data /= _scalar;
		return *this;
	}

	template< int L, typename T >
	cVector< L, T >& cVector< L, T >::normalize()
	{
		glm::normalize( m_data );
		return *this;
	}
	template< int L, typename T >
	cVector< L, T > cVector< L, T >::normalized() const
	{
		return glm::normalize( m_data );
	}

	template< int L, typename T >
	cVector< L, T >::cVector( const glm::vec< L, T >& _vector )
		: m_data( _vector )
	{}

	template< int L, typename T >
	cVector< L, T >::cVector( glm::vec< L, T >&& _vector )
		: m_data( std::move( _vector ) )
	{}
}