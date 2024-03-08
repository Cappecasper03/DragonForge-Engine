#pragma once

#include <glm/vec3.hpp>
#include <string>

#include "engine/misc/cColor.h"
#include "engine/rendering/assets/iQuad.h"
#include "sRenderAsset.h"

namespace df::opengl
{
	class cTexture;

	class cQuad : public sRenderAsset,
				  public iQuad
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( cQuad );

		cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white );
		~cQuad() override = default;

		void render() override;

	protected:
		void initOpenGL( const std::string& _texture_file );

		struct sVertex
		{
			glm::vec3 position;
			glm::vec2 tex_coord;
		};

		static bool m_initialized_once;
	};
}
