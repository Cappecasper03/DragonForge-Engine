#pragma once

namespace df
{
	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T >& cMatrixVectorProxy< C, R, T >::operator=( const cVector< R, T >& _vector )
	{
		m_matrix->m_data[ m_column_index ] = _vector.m_data;
		return *this;
	}
	template< int C, int R, typename T >
	template< int L >
	cMatrixVectorProxy< C, R, T >::operator cVector< L, T >() const
	{
		return cVector< L, T >( m_matrix->m_data[ m_column_index ] );
	}

	template< int C, int R, typename T >
	T& cMatrixVectorProxy< C, R, T >::operator[]( const int _index )
	{
		return m_matrix->m_data[ m_column_index ][ _index ];
	}
	template< int C, int R, typename T >
	const T& cMatrixVectorProxy< C, R, T >::operator[]( const int _index ) const
	{
		return m_matrix->m_data[ m_column_index ][ _index ];
	}

	template< int C, int R, typename T >
	T& cMatrixVectorProxy< C, R, T >::x()
		requires( R >= 1 )
	{
		return m_matrix->m_data[ m_column_index ].x;
	}
	template< int C, int R, typename T >
	const T& cMatrixVectorProxy< C, R, T >::x() const
		requires( R >= 1 )
	{
		return m_matrix->m_data[ m_column_index ].x;
	}

	template< int C, int R, typename T >
	T& cMatrixVectorProxy< C, R, T >::y()
		requires( R >= 2 )
	{
		return m_matrix->m_data[ m_column_index ].y;
	}
	template< int C, int R, typename T >
	const T& cMatrixVectorProxy< C, R, T >::y() const
		requires( R >= 2 )
	{
		return m_matrix->m_data[ m_column_index ].y;
	}

	template< int C, int R, typename T >
	T& cMatrixVectorProxy< C, R, T >::z()
		requires( R >= 3 )
	{
		return m_matrix->m_data[ m_column_index ].z;
	}
	template< int C, int R, typename T >
	const T& cMatrixVectorProxy< C, R, T >::z() const
		requires( R >= 3 )
	{
		return m_matrix->m_data[ m_column_index ].z;
	}

	template< int C, int R, typename T >
	T& cMatrixVectorProxy< C, R, T >::w()
		requires( R >= 4 )
	{
		return m_matrix->m_data[ m_column_index ].w;
	}
	template< int C, int R, typename T >
	const T& cMatrixVectorProxy< C, R, T >::w() const
		requires( R >= 4 )
	{
		return m_matrix->m_data[ m_column_index ].w;
	}

	template< int C, int R, typename T >
	cMatrixVectorProxy< C, R, T >::cMatrixVectorProxy( cMatrix< C, R, T >* _matrix, const int _index )
		: m_matrix( _matrix )
		, m_column_index( _index )
	{}
}