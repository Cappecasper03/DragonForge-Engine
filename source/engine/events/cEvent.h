#pragma once

#include <functional>

#include "engine/core/utils/Misc.h"
#include "iEvent.h"

namespace df
{
	template< typename... Targs >
	class cEvent final : public iEvent
	{
	public:
		DF_DeleteCopyAndMove( cEvent );

		cEvent()           = default;
		~cEvent() override = default;

		template< typename T >
		void subscribe( T* _object, void ( T::*_function )( Targs... ) );
		template< typename T >
		void subscribe( void ( *_function )( Targs... ) );

		void unsubscribe( void* _object ) override { m_function = nullptr; }

		void invoke( Targs... _args ) { m_function( _args... ); }

		bool isEmpty() const { return !m_function; }

	private:
		std::function< void( Targs... ) > m_function;
	};
}

#include "cEvent.inl"