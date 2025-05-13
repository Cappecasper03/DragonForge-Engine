#pragma once

#include "engine/core/math/cMatrix.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "engine/graphics/assets/iAsset.h"

namespace df
{
	class cTransform;

	class cCamera : public iAsset
	{
	public:
		DF_DisableCopyAndMove( cCamera );

		enum eType
		{
			kNone,
			kPerspective,
			kOrthographic,
		};

		enum eClearBuffer
		{
			kDepth   = 1 << 0,
			kStencil = 1 << 1,
			kColor   = 1 << 2,
		};

		explicit cCamera( std::string _name, eType _type, const cColor& _clear_color, float _fov, float _near_clip = .1f, float _far_clip = 10000 );
		~cCamera() override;

		void update( float _delta_time = 0 ) override;

		void beginRender( int _clear_buffers );
		void endRender();

		cMatrix4f m_view;
		cMatrix4f m_projection;
		cMatrix4f m_view_projection;

		cColor m_clear_color;

		eType       m_type;
		cTransform* m_transform;
		float       m_fov;
		float       m_aspect_ratio;
		float       m_near_clip;
		float       m_far_clip;
		cVector2f   m_orthographic_size;

	protected:
		void calculateProjection();

	private:
		void onWindowResize( int _width, int _height );

		cCamera* m_previous;
	};
}
