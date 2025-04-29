#pragma once

#include <glm/gtc/quaternion.hpp>

namespace df
{
	template< typename T >
	class cQuaternion
	{
	public:
		cQuaternion() = default;

		cQuaternion( const cQuaternion& _quaternion ) = default;
		cQuaternion( cQuaternion&& _quaternion )      = default;

		~cQuaternion() = default;

		cQuaternion& operator=( const cQuaternion& )        = default;
		cQuaternion& operator=( cQuaternion&& _quaternion ) = default;

		T&       operator[]( int _index );
		const T& operator[]( int _index ) const;

		T&       x();
		const T& x() const;

		T&       y();
		const T& y() const;

		T&       z();
		const T& z() const;

		T&       w();
		const T& w() const;

		cQuaternion operator+( const cQuaternion& _quaternion ) const { return cQuaternion( m_data + _quaternion.m_data ); }
		cQuaternion operator-( const cQuaternion& _quaternion ) const { return cQuaternion( m_data - _quaternion.m_data ); }
		cQuaternion operator*( const cQuaternion& _quaternion ) const { return cQuaternion( m_data * _quaternion.m_data ); }
		cQuaternion operator/( const cQuaternion& _quaternion ) const { return cQuaternion( m_data / _quaternion.m_data ); }

		cQuaternion& operator+=( const cQuaternion& _quaternion );
		cQuaternion& operator-=( const cQuaternion& _quaternion );
		cQuaternion& operator*=( const cQuaternion& _quaternion );
		cQuaternion& operator/=( const cQuaternion& _quaternion );

		cQuaternion operator+( T _scalar ) const { return cQuaternion( m_data + _scalar ); }
		cQuaternion operator-( T _scalar ) const { return cQuaternion( m_data - _scalar ); }
		cQuaternion operator*( T _scalar ) const { return cQuaternion( m_data * _scalar ); }
		cQuaternion operator/( T _scalar ) const { return cQuaternion( m_data / _scalar ); }

		cQuaternion& operator+=( T _scalar );
		cQuaternion& operator-=( T _scalar );
		cQuaternion& operator*=( T _scalar );
		cQuaternion& operator/=( T _scalar );

		cMatrix< 4, 4, T > toMatrix() const;

		static cQuaternion fromAngleAxis( T _radians, const cVector< 3, T >& _vector );

	private:
		cQuaternion( const glm::qua< T >& _quaternion );
		cQuaternion( glm::qua< T >&& _quaternion );

		glm::qua< T > m_data;
	};

	typedef cQuaternion< float > cQuaternionf;
}

#include "cQuaternion.inl"