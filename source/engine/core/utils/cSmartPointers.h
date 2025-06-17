#pragma once

#include <memory>

namespace df
{
	template< typename T >
	using cUnique = std::unique_ptr< T >;

	template< typename T, typename... Targs >
	constexpr cUnique< T > MakeUnique( Targs&&... args )
	{
		return std::make_unique< T >( std::forward< Targs >( args )... );
	}

	template< typename T >
	using cShared = std::shared_ptr< T >;

	template< typename T, typename... Targs >
	constexpr cShared< T > MakeShared( Targs&&... args )
	{
		return std::make_shared< T >( std::forward< Targs >( args )... );
	}

	template< typename T >
	using cWeak = std::weak_ptr< T >;
}