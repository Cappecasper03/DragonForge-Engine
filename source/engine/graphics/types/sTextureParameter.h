#pragma once

namespace df
{
	struct sTextureParameter
	{
		sTextureParameter() = delete;

		enum eName
		{
			kMinFilter,
			kMagFilter,

			kWrapS,
			kWrapT,
			kWrapR,
		};

		enum eParameter
		{
			kNearest,
			kLinear,
			kNearestMipmapNearest,
			kLinearMipmapNearest,
			kNearestMipmapLinear,
			kLinearMipmapLinear,

			kClampToEdge,
			kClampToBorder,
			kMirroredRepeat,
			kRepeat,
			kMirrorClampToEdge
		};

		static int toOpenGl( eName _name );
		static int toOpenGl( eParameter _parameter );
	};
}
