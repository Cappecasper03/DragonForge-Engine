#pragma once

namespace df
{
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
	cMatrix< 4, T > cQuaternion< T >::toMatrix() const
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