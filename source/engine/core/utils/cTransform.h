﻿#pragma once

#include <vector>

#include "engine/core/math/cMatrix.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	class cTransform final
	{
	public:
		DF_DefaultCopyAndMove( cTransform );

		cTransform();
		~cTransform();

		void update();

		bool addChild( cTransform& _child );
		bool removeChild( cTransform& _child );

		bool setParent( cTransform& _parent );
		bool removeParent();

		cMatrix4f m_local;
		cMatrix4f m_world;

		cTransform*                m_parent;
		std::vector< cTransform* > m_children;
	};
}
