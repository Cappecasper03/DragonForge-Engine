#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/vec2.hpp>

namespace df
{
	template< int I, typename T >
	class cVector
	{
		template< int J, typename R >
		friend class cVector;

	public:
		cVector();
		explicit cVector( T _scalar );

		cVector( const T& _x, const T& _y )
			requires( I == 2 );

		cVector( const T& _x, const T& _y, const T& _z )
			requires( I == 3 );

		cVector( const T& _x, const T& _y, const T& _z, const T& _w )
			requires( I == 4 );

		cVector( const cVector& ) = default;
		cVector( cVector&& )      = default;

		template< int J >
		explicit cVector( const cVector< J, T >& _vector )
			requires( J == I + 1 || J == I || J == I - 1 );

		template< int J >
		explicit cVector( const cVector< J, T >&& _vector )
			requires( J == I + 1 || J == I || J == I - 1 );

		template< int J >
		explicit cVector( const cVector< J, T >& _vector, const T& _value )
			requires( J == I - 1 );

		template< int J, typename R >
		explicit cVector( const cVector< J, R >& _vector )
			requires( J == I + 1 || J == I || J == I - 1 );

		template< int J, typename R >
		explicit cVector( const cVector< J, R >&& _vector )
			requires( J == I + 1 || J == I || J == I - 1 );

		template< int J, typename R >
		explicit cVector( const cVector< J, R >& _vector, const T& _value )
			requires( J == I - 1 );

		~cVector() = default;

		cVector& operator=( const cVector& ) = default;
		cVector& operator=( cVector&& )      = default;

		template< int Dim = I >
		T& x()
			requires( Dim >= 1 );
		template< int Dim = I >
		const T& x() const
			requires( Dim >= 1 );

		template< int Dim = I >
		T& y()
			requires( Dim >= 2 );
		template< int Dim = I >
		const T& y() const
			requires( Dim >= 2 );

		template< int Dim = I >
		T& z()
			requires( Dim >= 3 );
		template< int Dim = I >
		const T& z() const
			requires( Dim >= 3 );

		template< int Dim = I >
		T& w()
			requires( Dim >= 4 );
		template< int Dim = I >
		const T& w() const
			requires( Dim >= 4 );

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

		cVector  normalized() const;
		cVector& normalize();

		// START TEMP
		template< int J >
		explicit cVector( const glm::vec< J, T >& _vector )
			requires( J == I + 1 || J == I || J == I - 1 )
			: m_data( _vector )
		{}

		template< int J >
		explicit cVector( const glm::vec< J, T >&& _vector )
			requires( J == I + 1 || J == I || J == I - 1 )
			: m_data( std::move( _vector ) )
		{}

		template< int J, typename R >
		explicit cVector( const glm::vec< J, R >& _vector )
			requires( J == I + 1 || J == I || J == I - 1 )
			: m_data( _vector )
		{}

		template< int J, typename R >
		explicit cVector( const glm::vec< J, R >&& _vector )
			requires( J == I + 1 || J == I || J == I - 1 )
			: m_data( std::move( _vector ) )
		{}

		const glm::vec< I, T >& getGLM() const { return m_data; }
		glm::vec< I, T >&       getGLM() { return m_data; }
		// END TEMP

	private:
		cVector( const glm::vec< I, T >& _vector );
		cVector( glm::vec< I, T >&& _vector );

		glm::vec< I, T > m_data;
	};

	typedef cVector< 2, float >  cVector2f;
	typedef cVector< 2, double > cVector2d;
	typedef cVector< 2, int >    cVector2i;

	typedef cVector< 3, float >  cVector3f;
	typedef cVector< 3, double > cVector3d;
	typedef cVector< 3, int >    cVector3i;

	typedef cVector< 4, float >  cVector4f;
	typedef cVector< 4, double > cVector4d;
	typedef cVector< 4, int >    cVector4i;
}

#include "cVector.inl"