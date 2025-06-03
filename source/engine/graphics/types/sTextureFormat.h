#pragma once

namespace df
{
	struct sTextureFormat
	{
		sTextureFormat() = delete;

		enum eFormat
		{
			kRed,
			kRGB,
			kRGBA,

			kRGB16f,
		};

		static int toOpenGl( eFormat _format );
	};
}
