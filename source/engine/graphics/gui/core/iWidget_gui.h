#pragma once

#include <memory>

#include "engine/core/utils/cSmartPointers.h"
#include "engine/core/utils/Misc.h"

namespace df::gui
{
	class iWidget_gui : public std::enable_shared_from_this< iWidget_gui >
	{
	public:
		DF_DefaultCopyAndMove( iWidget_gui );

		iWidget_gui()          = default;
		virtual ~iWidget_gui() = default;

		virtual void paint() const = 0;

	protected:
		virtual void initialize() {}
	};

	template< typename T >
	class iWidgetCreate_gui : public iWidget_gui
	{
	public:
		DF_DefaultCopyAndMove( iWidgetCreate_gui );

		iWidgetCreate_gui()           = default;
		~iWidgetCreate_gui() override = default;

		static cShared< T > create()
		{
			cShared< iWidgetCreate_gui > widget = MakeShared< T >();
			widget->initialize();

			return std::static_pointer_cast< T >( widget );
		}

		static cShared< T > create( cShared< T >& _assign )
		{
			cShared< iWidgetCreate_gui > widget = MakeShared< T >();
			widget->initialize();

			_assign = std::static_pointer_cast< T >( widget );
			return _assign;
		}
	};
}
