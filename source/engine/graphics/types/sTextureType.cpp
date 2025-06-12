#include "sTextureType.h"

#include <glad/glad.h>

int df::sTextureType::toOpenGl( const eType _type )
{
	switch( _type )
	{
		case k2D: return GL_TEXTURE_2D;
	}

	return 0;
}