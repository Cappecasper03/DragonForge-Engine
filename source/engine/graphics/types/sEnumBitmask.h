#pragma once

#include <type_traits>

#include "engine/core/utils/Misc.h"

namespace df
{
	template< typename T >
	struct sEnumBitmask
	{
		DF_DeleteMove( sEnumBitmask );

		using eEnumType = std::underlying_type_t< T >;

		constexpr sEnumBitmask() noexcept;
		constexpr sEnumBitmask( eEnumType _value ) noexcept;

		constexpr sEnumBitmask( const sEnumBitmask& _right ) noexcept = default;

		constexpr ~sEnumBitmask() = default;

		constexpr sEnumBitmask& operator=( const sEnumBitmask& _right ) noexcept = default;

		constexpr sEnumBitmask& operator&=( const sEnumBitmask& _right ) const noexcept;
		constexpr sEnumBitmask& operator|=( const sEnumBitmask& _right ) const noexcept;

		constexpr sEnumBitmask operator&( const sEnumBitmask& _right ) const noexcept;
		constexpr sEnumBitmask operator|( const sEnumBitmask& _right ) const noexcept;

		constexpr bool operator==( const sEnumBitmask& _right ) const noexcept;
		constexpr bool operator!=( const sEnumBitmask& _right ) const noexcept;

		constexpr bool     operator!() const noexcept;
		explicit constexpr operator bool() const noexcept;

		eEnumType value;
	};
}

#include "sEnumBitmask.inl"
