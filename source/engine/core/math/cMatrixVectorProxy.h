#pragma once

namespace df
{
	template< int C, int R, typename T >
	class cMatrix;

	template< int L, typename T >
	class cVector;

	template< int C, int R, typename T >
	class cMatrixVectorProxy
	{
		friend class cMatrix< C, R, T >;

	public:
		cMatrixVectorProxy& operator=( const cVector< R, T >& _vector );

		template< int L >
		explicit operator cVector< L, T >() const;

		T&       operator[]( int _index );
		const T& operator[]( int _index ) const;

		T& x()
			requires( R >= 1 );
		const T& x() const
			requires( R >= 1 );

		T& y()
			requires( R >= 2 );
		const T& y() const
			requires( R >= 2 );

		T& z()
			requires( R >= 3 );
		const T& z() const
			requires( R >= 3 );

		T& w()
			requires( R >= 4 );
		const T& w() const
			requires( R >= 4 );

	private:
		cMatrix< C, R, T >* m_matrix;
		int                 m_column_index;

		cMatrixVectorProxy( cMatrix< C, R, T >* _matrix, int _index );
	};
}

#include "cMatrixVectorProxy.inl"