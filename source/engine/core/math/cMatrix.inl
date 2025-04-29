#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace df
{
	template< int C, int R, typename T >
	cMatrix< C, R, T >::cMatrix()
		: m_data( 1 )
	{}
	template< int C, int R, typename T >
	cMatrix< C, R, T >::cMatrix( T _scalar )
		: m_data( _scalar )
	{}
	template< int C, int R, typename T >
	cMatrix< C, R, T >::cMatrix( const cVector< R, T >& _right, const cVector< R, T >& _up, const cVector< R, T >& _backward )
		requires( C == 3 && R == 3 )
		: m_data( _right.m_data, _up.m_data, _backward.m_data )
	{}
	template< int C, int R, typename T >
	cMatrix< C, R, T >::cMatrix( const cVector< R, T >& _right, const cVector< R, T >& _up, const cVector< R, T >& _backward, const cVector< R, T >& _position )
		requires( C == 4 && R == 4 )
		: m_data( _right.m_data, _up.m_data, _backward.m_data, _position.m_data )
	{}

	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::operator[]( const int _index )
	{
		return cMatrixVectorProxy< C, R, T >( this, _index );
	}
	template< int C, int R, typename T >
	const cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::operator[]( const int _index ) const
	{
		return cMatrixVectorProxy< C, R, T >( this, _index );
	}

	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::right()
		requires( C == R && ( C == 3 || C == 4 ) )
	{
		return cMatrixVectorProxy< C, R, T >( this, 0 );
	}
	template< int C, int R, typename T >
	const cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::right() const
		requires( C == R && ( C == 3 || C == 4 ) )
	{
		return cMatrixVectorProxy< C, R, T >( this, 0 );
	}

	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::up()
		requires( C == R && ( C == 3 || C == 4 ) )
	{
		return cMatrixVectorProxy< C, R, T >( this, 1 );
	}
	template< int C, int R, typename T >
	const cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::up() const
		requires( C == R && ( C == 3 || C == 4 ) )
	{
		return cMatrixVectorProxy< C, R, T >( this, 1 );
	}

	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::backward()
		requires( C == R && ( C == 3 || C == 4 ) )
	{
		return cMatrixVectorProxy< C, R, T >( this, 2 );
	}
	template< int C, int R, typename T >
	const cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::backward() const
		requires( C == R && ( C == 3 || C == 4 ) )
	{
		return cMatrixVectorProxy< C, R, T >( this, 2 );
	}

	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::position()
		requires( C == 4 && R == 4 )
	{
		return cMatrixVectorProxy< C, R, T >( this, 3 );
	}
	template< int C, int R, typename T >
	const cMatrixVectorProxy< C, R, T > cMatrix< C, R, T >::position() const
		requires( C == 4 && R == 4 )
	{
		return cMatrixVectorProxy< C, R, T >( this, 3 );
	}

	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator+=( const cMatrix& _matrix )
	{
		m_data += _matrix.m_data;
		return *this;
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator-=( const cMatrix& _matrix )
	{
		m_data -= _matrix.m_data;
		return *this;
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator*=( const cMatrix& _matrix )
	{
		m_data *= _matrix.m_data;
		return *this;
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator/=( const cMatrix& _matrix )
	{
		m_data /= _matrix.m_data;
		return *this;
	}

	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator+=( T _scalar )
	{
		m_data += _scalar;
		return *this;
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator-=( T _scalar )
	{
		m_data -= _scalar;
		return *this;
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator*=( T _scalar )
	{
		m_data *= _scalar;
		return *this;
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T >& cMatrix< C, R, T >::operator/=( T _scalar )
	{
		m_data /= _scalar;
		return *this;
	}

	template< int C, int R, typename T >
	T* cMatrix< C, R, T >::data()
	{
		return glm::value_ptr( m_data );
	}
	template< int C, int R, typename T >
	T const* cMatrix< C, R, T >::data() const
	{
		return glm::value_ptr( m_data );
	}

	template< int C, int R, typename T >
	void cMatrix< C, R, T >::translate( const cVector< R - 1, T >& _vector )
		requires( C == 4 && R == 4 )
	{
		m_data = glm::translate( m_data, _vector.m_data );
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T > cMatrix< C, R, T >::translated( const cVector< R - 1, T >& _vector )
		requires( C == 4 && R == 4 )
	{
		return glm::translate( m_data, _vector.m_data );
	}

	template< int C, int R, typename T >
	void cMatrix< C, R, T >::rotate( T _radians, const cVector< R - 1, T >& _vector )
		requires( C == 4 && R == 4 )
	{
		m_data = glm::rotate( m_data, _radians, _vector.m_data );
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T > cMatrix< C, R, T >::rotated( T _radians, const cVector< R - 1, T >& _vector )
		requires( C == 4 && R == 4 )
	{
		return glm::rotate( m_data, _radians, _vector.m_data );
	}

	template< int C, int R, typename T >
	void cMatrix< C, R, T >::inverse()
	{
		m_data = glm::inverse( m_data );
	}
	template< int C, int R, typename T >
	cMatrix< C, R, T > cMatrix< C, R, T >::inversed()
	{
		return glm::inverse( m_data );
	}

	template< int C, int R, typename T >
	cMatrix< 4, 4, T > cMatrix< C, R, T >::createPerspective( T _radians, T _aspect_ratio, T _near_clip, T _far_clip )
	{
		return glm::perspective( _radians, _aspect_ratio, _near_clip, _far_clip );
	}

	template< int C, int R, typename T >
	cMatrix< 4, 4, T > cMatrix< C, R, T >::createOrtho( T _left, T _right, T _bottom, T _top, T _near_clip, T _far_clip )
	{
		return glm::ortho( _left, _right, _bottom, _top, _near_clip, _far_clip );
	}

	template< int C, int R, typename T >
	cMatrix< C, R, T >::cMatrix( const glm::mat< C, R, T >& _matrix )
		: m_data( _matrix )
	{}

	template< int C, int R, typename T >
	cMatrix< C, R, T >::cMatrix( glm::mat< C, R, T >&& _matrix )
		: m_data( std::move( _matrix ) )
	{}
}