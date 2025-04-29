#pragma once

#include <glm/mat4x4.hpp>

#include "cMatrixVectorProxy.h"

namespace df
{
	template< int C, int R, typename T >
	class cMatrix
	{
		friend class cMatrixVectorProxy< C, R, T >;

		template< typename T1 >
		friend class cQuaternion;

	public:
		cMatrix();
		explicit cMatrix( T _scalar );
		cMatrix( const cVector< R, T >& _right, const cVector< R, T >& _up, const cVector< R, T >& _backward )
			requires( C == 3 && R == 3 );
		cMatrix( const cVector< R, T >& _right, const cVector< R, T >& _up, const cVector< R, T >& _backward, const cVector< R, T >& _position )
			requires( C == 4 && R == 4 );

		cMatrix( const cMatrix& _matrix ) = default;
		cMatrix( cMatrix&& _matrix )      = default;

		~cMatrix() = default;

		cMatrix& operator=( const cMatrix& )    = default;
		cMatrix& operator=( cMatrix&& _matrix ) = default;

		cMatrixVectorProxy< C, R, T >       operator[]( int _index );
		const cMatrixVectorProxy< C, R, T > operator[]( int _index ) const;

		cMatrixVectorProxy< C, R, T > right()
			requires( C == R && ( C == 3 || C == 4 ) );
		const cMatrixVectorProxy< C, R, T > right() const
			requires( C == R && ( C == 3 || C == 4 ) );

		cMatrixVectorProxy< C, R, T > up()
			requires( C == R && ( C == 3 || C == 4 ) );
		const cMatrixVectorProxy< C, R, T > up() const
			requires( C == R && ( C == 3 || C == 4 ) );

		cMatrixVectorProxy< C, R, T > backward()
			requires( C == R && ( C == 3 || C == 4 ) );
		const cMatrixVectorProxy< C, R, T > backward() const
			requires( C == R && ( C == 3 || C == 4 ) );

		cMatrixVectorProxy< C, R, T > position()
			requires( C == 4 && R == 4 );
		const cMatrixVectorProxy< C, R, T > position() const
			requires( C == 4 && R == 4 );

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

		void translate( const cVector< R - 1, T >& _vector )
			requires( C == 4 && R == 4 );
		cMatrix translated( const cVector< R - 1, T >& _vector )
			requires( C == 4 && R == 4 );

		void rotate( T _radians, const cVector< R - 1, T >& _vector )
			requires( C == 4 && R == 4 );
		cMatrix rotated( T _radians, const cVector< R - 1, T >& _vector )
			requires( C == 4 && R == 4 );

		void    inverse();
		cMatrix inversed();

		static cMatrix< 4, 4, T > createPerspective( T _radians, T _aspect_ratio, T _near_clip, T _far_clip );
		static cMatrix< 4, 4, T > createOrtho( T _left, T _right, T _bottom, T _top, T _near_clip, T _far_clip );

	private:
		cMatrix( const glm::mat< C, R, T >& _matrix );
		cMatrix( glm::mat< C, R, T >&& _matrix );

		glm::mat< C, R, T > m_data;
	};

	typedef cMatrix< 2, 2, float > cMatrix2f;
	typedef cMatrix< 2, 3, float > cMatrix2x3f;
	typedef cMatrix< 2, 4, float > cMatrix2x4f;

	typedef cMatrix< 3, 2, float > cMatrix3x2f;
	typedef cMatrix< 3, 3, float > cMatrix3f;
	typedef cMatrix< 3, 4, float > cMatrix3x4f;

	typedef cMatrix< 4, 2, float > cMatrix4x2f;
	typedef cMatrix< 4, 3, float > cMatrix4x3f;
	typedef cMatrix< 4, 4, float > cMatrix4f;
}

#include "cMatrix.inl"