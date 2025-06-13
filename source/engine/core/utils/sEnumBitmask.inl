#pragma once

namespace df
{
	template< typename T >
	constexpr sEnumBitmask< T >::sEnumBitmask() noexcept
		: value( 0 )
	{}

	template< typename T >
	constexpr sEnumBitmask< T >::sEnumBitmask( const eEnumType _value ) noexcept
		: value( _value )
	{}

	template< typename T >
	constexpr sEnumBitmask< T >& sEnumBitmask< T >::operator&=( const sEnumBitmask& _right ) const noexcept
	{
		value &= _right.value;
		return *this;
	}

	template< typename T >
	constexpr sEnumBitmask< T >& sEnumBitmask< T >::operator|=( const sEnumBitmask& _right ) const noexcept
	{
		value |= _right.value;
		return *this;
	}

	template< typename T >
	constexpr sEnumBitmask< T > sEnumBitmask< T >::operator&( const sEnumBitmask& _right ) const noexcept
	{
		return value & _right.value;
	}

	template< typename T >
	constexpr sEnumBitmask< T > sEnumBitmask< T >::operator|( const sEnumBitmask& _right ) const noexcept
	{
		return value | _right.value;
	}

	template< typename T >
	constexpr bool sEnumBitmask< T >::operator==( const sEnumBitmask& _right ) const noexcept
	{
		return value == _right.value;
	}

	template< typename T >
	constexpr bool sEnumBitmask< T >::operator!=( const sEnumBitmask& _right ) const noexcept
	{
		return value != _right.value;
	}

	template< typename T >
	constexpr bool sEnumBitmask< T >::operator!() const noexcept
	{
		return value == 0;
	}

	template< typename T >
	constexpr sEnumBitmask< T >::operator bool() const noexcept
	{
		return value != 0;
	}
}