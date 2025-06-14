#pragma once

#include <memory>

namespace df
{
	template< typename T >
	using cUnique = std::unique_ptr< T >;

	template< typename T, typename... Args >
	constexpr cUnique< T > MakeUnique( Args&&... args )
	{
		return std::make_unique< T >( std::forward< Args >( args )... );
	}

	template< typename T >
	using cShared = std::shared_ptr< T >;

	template< typename T, typename... Args >
	constexpr cShared< T > MakeShared( Args&&... args )
	{
		return std::make_shared< T >( std::forward< Args >( args )... );
	}

	template< typename T >
	using cWeak = std::weak_ptr< T >;
}