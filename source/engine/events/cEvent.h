#pragma once

#include <functional>

#include "engine/misc/Misc.h"
#include "iEvent.h"

namespace df
{
	template< typename... Targs >
	class cEvent final : public iEvent
	{
	public:
		DF_DisableCopyAndMove( cEvent );

		cEvent()           = default;
		~cEvent() override = default;

		template< typename T >
		void subscribe( T* _object, void ( T::*_function )( Targs... ) );
		template< typename T >
		void subscribe( T* _object, void ( *_function )( Targs... ) );

		void unsubscribe( void* _object ) override;

		void invoke( Targs... _args );

	private:
		std::unordered_map< void*, std::function< void( Targs... ) > > m_subscribers;
	};
}

#include "cEvent.inl"