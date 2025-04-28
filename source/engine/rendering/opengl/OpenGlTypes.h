#pragma once

#include <glad/glad.h>

namespace df::opengl
{
	enum eDataType
	{
		kByte          = GL_BYTE,
		kUnsignedByte  = GL_UNSIGNED_BYTE,
		kShort         = GL_SHORT,
		kUnsignedShort = GL_UNSIGNED_SHORT,
		kInt           = GL_INT,
		kUnsignedInt   = GL_UNSIGNED_INT,
		kFloat         = GL_FLOAT,
	};
}