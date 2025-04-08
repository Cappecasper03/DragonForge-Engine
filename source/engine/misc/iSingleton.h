#pragma once

#include "Misc.h"

namespace df
{
	template< typename T >
	class iSingleton
	{
	public:
		DF_DisableCopyAndMove( iSingleton );

		iSingleton() { s_instance = reinterpret_cast< T* >( this ); }
		virtual ~iSingleton() { s_instance = nullptr; }

		template< typename... Targs >
		static T* initialize( Targs... _args );
		static void deinitialize();
		static T* getInstance() { return s_instance; }

	private:
		static T* s_instance;
	};
}

#include "iSingleton.inl"