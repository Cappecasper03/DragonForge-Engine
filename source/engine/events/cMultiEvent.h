#pragma once

#include <functional>

#include "engine/core/utils/Misc.h"
#include "iEvent.h"

namespace df
{
	template< typename... Targs >
	class cMultiEvent final : public iEvent
	{
	public:
		DF_DeleteCopyAndMove( cMultiEvent );

		cMultiEvent()           = default;
		~cMultiEvent() override = default;

		template< typename T >
		void subscribe( T* _object, void ( T::*_function )( Targs... ) );
		template< typename T >
		void subscribe( T* _object, void ( *_function )( Targs... ) );

		void unsubscribe( void* _object ) override { m_subscribers.erase( _object ); }
		void unsubscribeAll() { m_subscribers.clear(); }

		void invoke( Targs... _args );

		bool isBound() const { return !m_subscribers.empty(); }

	private:
		std::unordered_map< void*, std::function< void( Targs... ) > > m_subscribers;
	};
}

#include "cMultiEvent.inl"