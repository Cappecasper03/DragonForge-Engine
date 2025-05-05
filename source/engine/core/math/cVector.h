#pragma once

#include <glm/vec2.hpp>

namespace df
{
	template< int L, typename T >
	class cVector
	{
		template< int L1, typename T1 >
		friend class cVector;

		template< int C, int R, typename T1 >
		friend class cMatrix;

		template< int C, int R, typename T1 >
		friend class cMatrixVectorProxy;

		template< typename T1 >
		friend class cQuaternion;

	public:
		cVector();
		cVector( T _scalar );

		cVector( const T& _x, const T& _y )
			requires( L == 2 );
		cVector( const T& _x, const T& _y, const T& _z )
			requires( L == 3 );
		cVector( const T& _x, const T& _y, const T& _z, const T& _w )
			requires( L == 4 );

		template< int L1 >
		cVector( const cVector< L1, T >& _vector )
			requires( L1 == L + 1 || L1 == L || L1 == L - 1 );

		template< int L1 >
		cVector( const cVector< L1, T >&& _vector )
			requires( L1 == L + 1 || L1 == L || L1 == L - 1 );

		template< int L1 >
		cVector( const cVector< L1, T >& _vector, const T& _value )
			requires( L1 == L - 1 );

		template< int L1, typename T1 >
		cVector( const cVector< L1, T1 >& _vector )
			requires( L1 == L + 1 || L1 == L || L1 == L - 1 );

		template< int L1, typename T1 >
		cVector( const cVector< L1, T1 >&& _vector )
			requires( L1 == L + 1 || L1 == L || L1 == L - 1 );

		template< int L1, typename T1 >
		cVector( const cVector< L1, T1 >& _vector, const T& _value )
			requires( L1 == L - 1 );

		cVector( const cVector& ) = default;
		cVector( cVector&& )      = default;

		~cVector() = default;

		cVector& operator=( const cVector& ) = default;
		cVector& operator=( cVector&& )      = default;

		T&       operator[]( int _index );
		const T& operator[]( int _index ) const;

		T& x()
			requires( L >= 1 );
		const T& x() const
			requires( L >= 1 );

		T& y()
			requires( L >= 2 );
		const T& y() const
			requires( L >= 2 );

		T& z()
			requires( L >= 3 );
		const T& z() const
			requires( L >= 3 );

		T& w()
			requires( L >= 4 );
		const T& w() const
			requires( L >= 4 );

		cVector operator+( const cVector& _vector ) const { return cVector( m_data + _vector.m_data ); }
		cVector operator-( const cVector& _vector ) const { return cVector( m_data - _vector.m_data ); }
		cVector operator*( const cVector& _vector ) const { return cVector( m_data * _vector.m_data ); }
		cVector operator/( const cVector& _vector ) const { return cVector( m_data / _vector.m_data ); }

		cVector& operator+=( const cVector& _vector );
		cVector& operator-=( const cVector& _vector );
		cVector& operator*=( const cVector& _vector );
		cVector& operator/=( const cVector& _vector );

		cVector operator+( T _scalar ) const { return cVector( m_data + _scalar ); }
		cVector operator-( T _scalar ) const { return cVector( m_data - _scalar ); }
		cVector operator*( T _scalar ) const { return cVector( m_data * _scalar ); }
		cVector operator/( T _scalar ) const { return cVector( m_data / _scalar ); }

		cVector& operator+=( T _scalar );
		cVector& operator-=( T _scalar );
		cVector& operator*=( T _scalar );
		cVector& operator/=( T _scalar );

		cVector& normalize();
		cVector  normalized() const;

	private:
		cVector( const glm::vec< L, T >& _vector );
		cVector( glm::vec< L, T >&& _vector );

		glm::vec< L, T > m_data;
	};

	typedef cVector< 2, float > cVector2f;
	typedef cVector< 2, int >   cVector2i;

	typedef cVector< 3, float > cVector3f;
	typedef cVector< 3, int >   cVector3i;

	typedef cVector< 4, float > cVector4f;
	typedef cVector< 4, int >   cVector4i;
}

#include "cVector.inl"