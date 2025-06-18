#pragma once

#include "engine/graphics/gui/core/cWidget_gui.h"
#include "engine/graphics/gui/core/iWidget_gui.h"

namespace df::gui
{
	class cOverlay_gui : public iWidgetCreate_gui< cOverlay_gui >
	{
	public:
		DF_DefaultCopyAndMove( cOverlay_gui );

		cOverlay_gui()           = default;
		~cOverlay_gui() override = default;

		cShared< cOverlay_gui > color( const cColor& _color );

		cShared< cOverlay_gui > widthFit( float _min, float _max );
		cShared< cOverlay_gui > widthFit( float _min_max );
		cShared< cOverlay_gui > widthFit();
		cShared< cOverlay_gui > widthGrow( float _min, float _max );
		cShared< cOverlay_gui > widthGrow( float _min_max );
		cShared< cOverlay_gui > widthGrow();
		cShared< cOverlay_gui > widthFixed( float _width );
		cShared< cOverlay_gui > widthPercent( float _percent );

		cShared< cOverlay_gui > heightFit( float _min, float _max );
		cShared< cOverlay_gui > heightFit( float _min_max );
		cShared< cOverlay_gui > heightFit();
		cShared< cOverlay_gui > heightGrow( float _min, float _max );
		cShared< cOverlay_gui > heightGrow( float _min_max );
		cShared< cOverlay_gui > heightGrow();
		cShared< cOverlay_gui > heightFixed( float _height );
		cShared< cOverlay_gui > heightPercent( float _percent );

		cShared< cOverlay_gui > padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		cShared< cOverlay_gui > padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		cShared< cOverlay_gui > padding( std::uint16_t _padding );

		cShared< cOverlay_gui > margin( std::uint16_t _margin );

		cShared< cOverlay_gui > borderColor( const cColor& _color );
		cShared< cOverlay_gui > borderWidth( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom, std::uint16_t _between_children );
		cShared< cOverlay_gui > borderWidth( std::uint16_t _left_right, std::uint16_t _top_bottom, std::uint16_t _between_children );
		cShared< cOverlay_gui > borderWidth( std::uint16_t _width, std::uint16_t _between_children );

		cShared< cOverlay_gui > floatingOffset( const cVector2f& _offset );
		cShared< cOverlay_gui > floatingExpand( const cVector2f& _expand );
		cShared< cOverlay_gui > floatingElementId( const cWidget_gui* _element_id );
		cShared< cOverlay_gui > floatingDepthIndex( std::int16_t _index );
		cShared< cOverlay_gui > floatingAttachPoints( cWidget_gui::eAttachPoint _element, cWidget_gui::eAttachPoint _parent );
		cShared< cOverlay_gui > floatingInputMode( cWidget_gui::eInputMode _mode );
		cShared< cOverlay_gui > floatingAttachMode( cWidget_gui::eAttachMode _mode );
		cShared< cOverlay_gui > floatingClipToParent( bool _clip_to_parent );

		cShared< cOverlay_gui > setContent( const cShared< iWidget_gui >& _widget );

		void paint() const override;

	private:
		void initialize() override;

		cWidget_gui m_data;

		cShared< iWidget_gui > m_content;
	};
}
