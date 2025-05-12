#pragma once

#include <string>
#include <unordered_map>

#include "engine/core/utils/iSingleton.h"

namespace df
{
	struct iEvent;

	namespace event
	{
		static std::string input            = "input";
		static std::string update           = "update";
		static std::string render_3d        = "render_3d";
		static std::string render_2d        = "render_2d";
		static std::string imgui            = "imgui";
		static std::string on_window_resize = "on_window_resize";
	}

	class cEventManager final : public iSingleton< cEventManager >
	{
	public:
		DF_DisableCopyAndMove( cEventManager );

		cEventManager() = default;
		~cEventManager() override;

		template< typename T, typename... Targs >
		static void subscribe( const std::string& _name, T* _object, void ( T::*_function )( Targs... ) );
		template< typename T, typename... Targs >
		static void subscribe( const std::string& _name, T* _object, void ( *_function )( Targs... ) );

		template< typename T >
		static void unsubscribe( const std::string& _name, T* _object );

		template< typename... Targs >
		static void invoke( const std::string& _name, Targs... _args );

	private:
		std::unordered_map< std::string, iEvent* > m_events;
	};
}

#include "cEventManager.inl"
