#pragma once

namespace df
{
	template< int L, typename T >
	class cMatrix;

	template< int L, typename T >
	class cVector;

	template< int L, typename T >
	class cMatrixVectorProxy
	{
		friend class cMatrix< L, T >;

	public:
		cMatrixVectorProxy& operator=( const cVector< L, T >& _vector );

		template< int L1 >
		explicit operator cVector< L1, T >() const
		{
			return cVector< L1, T >( m_matrix->m_data[ m_column_index ] );
		}

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

	private:
		cMatrix< L, T >* m_matrix;
		int              m_column_index;

		cMatrixVectorProxy( cMatrix< L, T >* _matrix, int _index );
	};
}

#include "cMatrixVectorProxy.inl"