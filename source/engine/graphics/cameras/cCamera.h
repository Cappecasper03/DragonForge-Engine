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

		cMatrix4f view;
		cMatrix4f projection;
		cMatrix4f view_projection;

		cColor clear_color;

		eType       type;
		cTransform* transform;
		float       fov;
		float       aspect_ratio;
		float       near_clip;
		float       far_clip;
		cVector2f   ortographic_size;

	protected:
		void calculateProjection();

	private:
		void onWindowResize( int _width, int _height );

		cCamera* m_previus;
	};
}
