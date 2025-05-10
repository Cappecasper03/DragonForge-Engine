#pragma once

#include <vector>

#include "engine/core/math/cMatrix.h"
#include "engine/core/utils/Misc.h"

namespace df
{
	class cTransform final
	{
	public:
		DF_DisableCopy( cTransform );

		cTransform();
		cTransform( cTransform&& _other ) = default;
		~cTransform();

		cTransform& operator=( cTransform&& _other ) = default;

		void update();

		bool addChild( cTransform& _child );
		bool removeChild( cTransform& _child );

		bool setParent( cTransform& _parent );
		bool removeParent();

		cMatrix4f local;
		cMatrix4f world;

		cTransform*                parent;
		std::vector< cTransform* > children;
	};
}
