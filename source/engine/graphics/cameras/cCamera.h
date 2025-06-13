#pragma once

#include "engine/core/math/cMatrix.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/core/utils/cTransform.h"
#include "engine/core/utils/sEnumBitmask.h"
#include "engine/graphics/assets/iObject.h"

namespace df
{
	class cTransform;

	class cCamera
	{
	public:
		DF_DeleteCopyAndMove( cCamera );

		enum eType
		{
			kPerspective,
			kOrthographic,
			kNone,
		};

		enum eClear
		{
			kDepth   = 1 << 0,
			kStencil = 1 << 1,
			kColor   = 1 << 2,
		};

		using eClearFlags = sEnumBitmask< eClear >;

		struct sDescription
		{
			std::string name;
			eType       type        = kPerspective;
			cColor      clear_color = color::black;
			float       fov         = 90;
			float       near_clip   = .1f;
			float       far_clip    = 10000;
		};

		cCamera( const sDescription& _description );
		virtual ~cCamera() = default;

		virtual void update( float _delta_time = 0 );

		virtual void beginRender( eClearFlags _clear_flags );
		virtual void endRender();

		eType getType() const { return m_description.type; }

		cMatrix4f m_view;
		cMatrix4f m_projection;
		cMatrix4f m_view_projection;

		cTransform m_transform;
		float      m_aspect_ratio;
		cVector2f  m_orthographic_size;

		bool m_flip_y;

	protected:
		void calculateProjection();

		void onWindowResize( int _width, int _height );

		sDescription m_description;

		cCamera* m_previous;
	};
}
