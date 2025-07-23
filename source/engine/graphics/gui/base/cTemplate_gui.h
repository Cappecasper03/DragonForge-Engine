#pragma once

#include "engine/graphics/gui/core/cWidget_gui.h"
#include "engine/graphics/gui/core/iWidget_gui.h"

namespace df::gui
{
	class cTemplate_gui : public iWidgetCreate_gui< cTemplate_gui >
	{
	public:
		DF_DefaultCopyAndMove( cTemplate_gui );

		cTemplate_gui()           = default;
		~cTemplate_gui() override = default;

		cShared< cTemplate_gui > id( const std::string& _id );

		cShared< cTemplate_gui > widthFit( float _min, float _max );
		cShared< cTemplate_gui > widthFit( float _min_max );
		cShared< cTemplate_gui > widthFit();
		cShared< cTemplate_gui > widthGrow( float _min, float _max );
		cShared< cTemplate_gui > widthGrow( float _min_max );
		cShared< cTemplate_gui > widthGrow();
		cShared< cTemplate_gui > widthFixed( float _width );
		cShared< cTemplate_gui > widthPercent( float _percent );

		cShared< cTemplate_gui > heightFit( float _min, float _max );
		cShared< cTemplate_gui > heightFit( float _min_max );
		cShared< cTemplate_gui > heightFit();
		cShared< cTemplate_gui > heightGrow( float _min, float _max );
		cShared< cTemplate_gui > heightGrow( float _min_max );
		cShared< cTemplate_gui > heightGrow();
		cShared< cTemplate_gui > heightFixed( float _height );
		cShared< cTemplate_gui > heightPercent( float _percent );

		cShared< cTemplate_gui > padding( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom );
		cShared< cTemplate_gui > padding( std::uint16_t _left_right, std::uint16_t _top_bottom );
		cShared< cTemplate_gui > padding( std::uint16_t _padding );

		cShared< cTemplate_gui > margin( std::uint16_t _margin );

		cShared< cTemplate_gui > alignment( cWidget_gui::eHorizontalAlignment _alignment );
		cShared< cTemplate_gui > alignment( cWidget_gui::eVerticalAlignment _alignment );
		cShared< cTemplate_gui > alignment( cWidget_gui::eHorizontalAlignment _horizontal, cWidget_gui::eVerticalAlignment _vertical );

		cShared< cTemplate_gui > direction( cWidget_gui::eDirection _direction );

		cShared< cTemplate_gui > color( const cColor& _color );

		cShared< cTemplate_gui > cornerRadius( float _top_left, float _top_right, float _bottom_left, float _bottom_right );
		cShared< cTemplate_gui > cornerRadius( float _top, float _bottom );
		cShared< cTemplate_gui > cornerRadius( float _radius );

		cShared< cTemplate_gui > image( cTexture2D* _texture );
		cShared< cTemplate_gui > image( cTexture2D* _texture, const cVector2f& _size );

		cShared< cTemplate_gui > floatingOffset( const cVector2f& _offset );
		cShared< cTemplate_gui > floatingExpand( const cVector2f& _expand );
		cShared< cTemplate_gui > floatingElementId( const cWidget_gui* _element_id );
		cShared< cTemplate_gui > floatingDepthIndex( std::int16_t _index );
		cShared< cTemplate_gui > floatingAttachPoints( cWidget_gui::eAttachPoint _element, cWidget_gui::eAttachPoint _parent );
		cShared< cTemplate_gui > floatingInputMode( cWidget_gui::eInputMode _mode );
		cShared< cTemplate_gui > floatingAttachMode( cWidget_gui::eAttachMode _mode );
		cShared< cTemplate_gui > floatingClipToParent( bool _clip_to_parent );

		cShared< cTemplate_gui > borderColor( const cColor& _color );
		cShared< cTemplate_gui > borderWidth( std::uint16_t _left, std::uint16_t _right, std::uint16_t _top, std::uint16_t _bottom, std::uint16_t _between_children );
		cShared< cTemplate_gui > borderWidth( std::uint16_t _left_right, std::uint16_t _top_bottom, std::uint16_t _between_children );
		cShared< cTemplate_gui > borderWidth( std::uint16_t _width, std::uint16_t _between_children );

		void paint() const override;

	private:
		void initialize() override;

		cWidget_gui m_data;
	};
}
