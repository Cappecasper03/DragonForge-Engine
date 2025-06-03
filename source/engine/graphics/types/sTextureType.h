#pragma once

namespace df
{
	struct sTextureType
	{
		sTextureType() = delete;

		enum eType
		{
			k2D,
		};

		static int toOpenGl( eType _type );
	};
}
