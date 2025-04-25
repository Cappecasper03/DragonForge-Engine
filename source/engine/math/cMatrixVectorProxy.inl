#pragma once

namespace df
{
	template< int L, typename T >
	cMatrixVectorProxy< L, T >& cMatrixVectorProxy< L, T >::operator=( const cVector< L, T >& _vector )
	{
		m_matrix->m_data[ m_column_index ] = _vector.m_data;
		return *this;
	}

	template< int L, typename T >
	T& cMatrixVectorProxy< L, T >::x()
		requires( L >= 1 )
	{
		return m_matrix->m_data[ m_column_index ].x;
	}
	template< int L, typename T >
	const T& cMatrixVectorProxy< L, T >::x() const
		requires( L >= 1 )
	{
		return m_matrix->m_data[ m_column_index ].x;
	}

	template< int L, typename T >
	T& cMatrixVectorProxy< L, T >::y()
		requires( L >= 2 )
	{
		return m_matrix->m_data[ m_column_index ].y;
	}
	template< int L, typename T >
	const T& cMatrixVectorProxy< L, T >::y() const
		requires( L >= 2 )
	{
		return m_matrix->m_data[ m_column_index ].y;
	}

	template< int L, typename T >
	T& cMatrixVectorProxy< L, T >::z()
		requires( L >= 3 )
	{
		return m_matrix->m_data[ m_column_index ].z;
	}
	template< int L, typename T >
	const T& cMatrixVectorProxy< L, T >::z() const
		requires( L >= 3 )
	{
		return m_matrix->m_data[ m_column_index ].z;
	}

	template< int L, typename T >
	T& cMatrixVectorProxy< L, T >::w()
		requires( L >= 4 )
	{
		return m_matrix->m_data[ m_column_index ].w;
	}
	template< int L, typename T >
	const T& cMatrixVectorProxy< L, T >::w() const
		requires( L >= 4 )
	{
		return m_matrix->m_data[ m_column_index ].w;
	}

	template< int L, typename T >
	cMatrixVectorProxy< L, T >::cMatrixVectorProxy( cMatrix< L, T >* _matrix, const int _index )
		: m_matrix( _matrix )
		, m_column_index( _index )
	{}
}