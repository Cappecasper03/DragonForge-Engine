#pragma once

namespace df
{
	template< typename T >
	T& cQuaternion< T >::operator[]( const int _index )
	{
		return m_data[ _index ];
	}
	template< typename T >
	const T& cQuaternion< T >::operator[]( const int _index ) const
	{
		return m_data[ _index ];
	}

	template< typename T >
	T& cQuaternion< T >::x()
	{
		return m_data.x;
	}
	template< typename T >
	const T& cQuaternion< T >::x() const
	{
		return m_data.x;
	}

	template< typename T >
	T& cQuaternion< T >::y()
	{
		return m_data.y;
	}
	template< typename T >
	const T& cQuaternion< T >::y() const
	{
		return m_data.y;
	}

	template< typename T >
	T& cQuaternion< T >::z()
	{
		return m_data.z;
	}
	template< typename T >
	const T& cQuaternion< T >::z() const
	{
		return m_data.z;
	}

	template< typename T >
	T& cQuaternion< T >::w()
	{
		return m_data.w;
	}
	template< typename T >
	const T& cQuaternion< T >::w() const
	{
		return m_data.w;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator+=( const cQuaternion& _quaternion )
	{
		m_data += _quaternion.m_data;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator-=( const cQuaternion& _quaternion )
	{
		m_data -= _quaternion.m_data;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator*=( const cQuaternion& _quaternion )
	{
		m_data *= _quaternion.m_data;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator/=( const cQuaternion& _quaternion )
	{
		m_data /= _quaternion.m_data;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator+=( T _scalar )
	{
		m_data += _scalar;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator-=( T _scalar )
	{
		m_data -= _scalar;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator*=( T _scalar )
	{
		m_data *= _scalar;
		return *this;
	}

	template< typename T >
	cQuaternion< T >& cQuaternion< T >::operator/=( T _scalar )
	{
		m_data /= _scalar;
		return *this;
	}

	template< typename T >
	cMatrix< 4, 4, T > cQuaternion< T >::toMatrix() const
	{
		return glm::mat4_cast( m_data );
	}

	template< typename T >
	cQuaternion< T > cQuaternion< T >::fromAngleAxis( T _radians, const cVector< 3, T >& _vector )
	{
		return glm::angleAxis( _radians, _vector.m_data );
	}

	template< typename T >
	cQuaternion< T >::cQuaternion( const glm::qua< T >& _quaternion )
		: m_data( _quaternion )
	{}

	template< typename T >
	cQuaternion< T >::cQuaternion( glm::qua< T >&& _quaternion )
		: m_data( std::move( _quaternion ) )
	{}
}