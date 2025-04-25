#pragma once

#include <glm/mat4x4.hpp>

#include "cMatrixVectorProxy.h"

namespace df
{
	template< int L, typename T >
	class cMatrix
	{
		friend class cMatrixVectorProxy< L, T >;

		template< typename T1 >
		friend class cQuaternion;

	public:
		cMatrix();
		explicit cMatrix( T _scalar );
		cMatrix( const cVector< L, T >& _right, const cVector< L, T >& _up, const cVector< L, T >& _backward, const cVector< L, T >& _position );

		cMatrix( const cMatrix& _matrix ) = default;
		cMatrix( cMatrix&& _matrix )      = default;

		~cMatrix() = default;

		cMatrix& operator=( const cMatrix& )    = default;
		cMatrix& operator=( cMatrix&& _matrix ) = default;

		cMatrixVectorProxy< 4, T > right()
			requires( L == 4 );
		cMatrixVectorProxy< 4, T > up()
			requires( L == 4 );
		cMatrixVectorProxy< 4, T > backward()
			requires( L == 4 );
		cMatrixVectorProxy< 4, T > position()
			requires( L == 4 );

		cMatrix operator+( const cMatrix& _matrix ) const { return cMatrix( m_data + _matrix.m_data ); }
		cMatrix operator-( const cMatrix& _matrix ) const { return cMatrix( m_data - _matrix.m_data ); }
		cMatrix operator*( const cMatrix& _matrix ) const { return cMatrix( m_data * _matrix.m_data ); }
		cMatrix operator/( const cMatrix& _matrix ) const { return cMatrix( m_data / _matrix.m_data ); }

		cMatrix& operator+=( const cMatrix& _matrix );
		cMatrix& operator-=( const cMatrix& _matrix );
		cMatrix& operator*=( const cMatrix& _matrix );
		cMatrix& operator/=( const cMatrix& _matrix );

		cMatrix operator+( T _scalar ) const { return cMatrix( m_data + _scalar ); }
		cMatrix operator-( T _scalar ) const { return cMatrix( m_data - _scalar ); }
		cMatrix operator*( T _scalar ) const { return cMatrix( m_data * _scalar ); }
		cMatrix operator/( T _scalar ) const { return cMatrix( m_data / _scalar ); }

		cMatrix& operator+=( T _scalar );
		cMatrix& operator-=( T _scalar );
		cMatrix& operator*=( T _scalar );
		cMatrix& operator/=( T _scalar );

		T*       data();
		T const* data() const;

		void    translate( const cVector< L - 1, T >& _vector );
		cMatrix translated( const cVector< L - 1, T >& _vector );

		void    rotate( T _radians, const cVector< L - 1, T >& _vector );
		cMatrix rotated( T _radians, const cVector< L - 1, T >& _vector );

		void    inverse();
		cMatrix inversed();

		static cMatrix fromPerspective( T _radians, T _aspect_ratio, T _near_clip, T _far_clip );
		static cMatrix fromOrtho( T _left, T _right, T _bottom, T _top, T _near_clip, T _far_clip );

	private:
		cMatrix( const glm::mat< L, L, T >& _matrix );
		cMatrix( glm::mat< L, L, T >&& _matrix );

		glm::mat< L, L, T > m_data;
	};

	typedef cMatrix< 4, float > cMatrix4f;
}

#include "cMatrix.inl"