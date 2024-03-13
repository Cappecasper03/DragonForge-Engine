﻿#pragma once

#include <glm/vec3.hpp>

#include "AssetTypes.h"
#include "engine/misc/cColor.h"

namespace df
{
	class iTexture;

	class iQuad : public iRenderAsset
	{
	public:
		DF_DISABLE_COPY_AND_MOVE( iQuad );

		struct sVertex
		{
			glm::vec3 position;
			glm::vec2 tex_coord;
		};

		iQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color = color::white );
		~iQuad() override = default;

		const std::vector< sVertex >&  getVertices() const { return m_vertices; }
		const std::vector< unsigned >& getIndices() const { return m_indices; }

		iTexture* texture;
		cColor    color;

	protected:
		std::vector< sVertex >  m_vertices;
		std::vector< unsigned > m_indices;
	};
}
