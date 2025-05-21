#pragma once

#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"
#include "InputTypes.h"

namespace df
{
	class iPlayerController
	{
	public:
		DF_DeleteCopyAndMove( iPlayerController );

		iPlayerController()          = default;
		virtual ~iPlayerController() = default;

		void setActive( const bool _active )
		{
			DF_ProfilingScopeCpu;

			m_active = _active;
			if( m_active )
				cEventManager::subscribe( event::input, this, &iPlayerController::input );
			else
				cEventManager::unsubscribe( event::input, this );
		}

		virtual void update( float /*_delta_time*/ ) {}
		virtual void input( const input::sInputs& _input ) = 0;

	protected:
		bool m_active;
	};
}
