#include "cFloating_gui.h"

#include "cWidget_gui.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::gui
{
	cFloating_gui& cFloating_gui::offset( const cVector2f& _offset )
	{
		DF_ProfilingScopeCpu;

		m_data.offset.x = _offset.x();
		m_data.offset.y = _offset.y();

		return *this;
	}

	cFloating_gui& cFloating_gui::expand( const cVector2f& _expand )
	{
		DF_ProfilingScopeCpu;

		m_data.expand.width  = _expand.width();
		m_data.expand.height = _expand.height();

		return *this;
	}

	cFloating_gui& cFloating_gui::elementId( const cWidget_gui* _element_id )
	{
		DF_ProfilingScopeCpu;

		m_data.parentId = _element_id->m_data.id.id;

		return *this;
	}

	cFloating_gui& cFloating_gui::depthIndex( const std::int16_t _index )
	{
		DF_ProfilingScopeCpu;

		m_data.zIndex = _index;

		return *this;
	}

	cFloating_gui& cFloating_gui::attachPoints( const eAttachPoint _element, const eAttachPoint _parent )
	{
		DF_ProfilingScopeCpu;

		switch( _element )
		{
			case kLeftTop:      m_data.attachPoints.element = CLAY_ATTACH_POINT_LEFT_TOP; break;
			case kLeftCenter:   m_data.attachPoints.element = CLAY_ATTACH_POINT_LEFT_CENTER; break;
			case kLeftBottom:   m_data.attachPoints.element = CLAY_ATTACH_POINT_LEFT_BOTTOM; break;
			case kCenterTop:    m_data.attachPoints.element = CLAY_ATTACH_POINT_CENTER_TOP; break;
			case kCenter:       m_data.attachPoints.element = CLAY_ATTACH_POINT_CENTER_CENTER; break;
			case kCenterBottom: m_data.attachPoints.element = CLAY_ATTACH_POINT_CENTER_BOTTOM; break;
			case kRightTop:     m_data.attachPoints.element = CLAY_ATTACH_POINT_RIGHT_TOP; break;
			case kRightCenter:  m_data.attachPoints.element = CLAY_ATTACH_POINT_RIGHT_CENTER; break;
			case kRightBottom:  m_data.attachPoints.element = CLAY_ATTACH_POINT_RIGHT_BOTTOM; break;
		}

		switch( _parent )
		{
			case kLeftTop:      m_data.attachPoints.parent = CLAY_ATTACH_POINT_LEFT_TOP; break;
			case kLeftCenter:   m_data.attachPoints.parent = CLAY_ATTACH_POINT_LEFT_CENTER; break;
			case kLeftBottom:   m_data.attachPoints.parent = CLAY_ATTACH_POINT_LEFT_BOTTOM; break;
			case kCenterTop:    m_data.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_TOP; break;
			case kCenter:       m_data.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_CENTER; break;
			case kCenterBottom: m_data.attachPoints.parent = CLAY_ATTACH_POINT_CENTER_BOTTOM; break;
			case kRightTop:     m_data.attachPoints.parent = CLAY_ATTACH_POINT_RIGHT_TOP; break;
			case kRightCenter:  m_data.attachPoints.parent = CLAY_ATTACH_POINT_RIGHT_CENTER; break;
			case kRightBottom:  m_data.attachPoints.parent = CLAY_ATTACH_POINT_RIGHT_BOTTOM; break;
		}

		return *this;
	}

	cFloating_gui& cFloating_gui::inputMode( const eInputMode _mode )
	{
		DF_ProfilingScopeCpu;

		switch( _mode )
		{
			case kCapture:     m_data.pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE; break;
			case kPassthrough: m_data.pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH; break;
		}

		return *this;
	}

	cFloating_gui& cFloating_gui::attachMode( const eAttachMode _mode )
	{
		DF_ProfilingScopeCpu;

		switch( _mode )
		{
			case kNone:    m_data.attachTo = CLAY_ATTACH_TO_NONE; break;
			case kParent:  m_data.attachTo = CLAY_ATTACH_TO_PARENT; break;
			case kElement: m_data.attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID; break;
			case kRoot:    m_data.attachTo = CLAY_ATTACH_TO_ROOT; break;
		}

		return *this;
	}

	cFloating_gui& cFloating_gui::clipToParent( const bool _clip_to_parent )
	{
		DF_ProfilingScopeCpu;

		m_data.clipTo = _clip_to_parent ? CLAY_CLIP_TO_ATTACHED_PARENT : CLAY_CLIP_TO_NONE;

		return *this;
	}
}