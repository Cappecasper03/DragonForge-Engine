#pragma once

#include <string>
#include <unordered_map>

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/iSingleton.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	struct iRenderCallback;

	class iObject;

	template< typename T, typename Tasset >
	class iObjectManager : public iSingleton< T >
	{
	public:
		DF_DeleteCopyAndMove( iObjectManager );

		iObjectManager()           = default;
		~iObjectManager() override = default;

		template< typename Ttype = Tasset, typename... Targs >
		static Tasset* create( const std::string& _name, Targs... _args );

		static bool add( Tasset* _asset );

		static void update( float _delta_time );

		static bool destroy( const std::string& _name );
		static bool destroy( const Tasset* _asset );
		static void clear();

		static Tasset* get( const std::string& _name );

	protected:
		std::unordered_map< std::string, cUnique< iObject > > m_objects;
	};
}

#include "iObjectManager.inl"