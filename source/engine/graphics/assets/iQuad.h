#pragma once

#include <vector>

#include "engine/core/math/cMatrix.h"
#include "engine/core/math/cVector.h"
#include "engine/core/utils/cColor.h"
#include "iAsset.h"
#include "textures/cTexture2D.h"

namespace df
{
	class iQuad : public iAsset
	{
	public:
		DF_DeleteCopyAndMove( iQuad );

		struct sPushConstants
		{
			cMatrix4f world_matrix;
		};

		struct sVertex
		{
			cVector3f position  = cVector3f( 0 );
			cVector2f tex_coord = cVector2f( 0 );
		};

		iQuad( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white );
		~iQuad() override = default;

		virtual bool loadTexture( const std::string& _file_path, bool _mipmapped = true, int _mipmaps = 0, bool _flip_vertically_on_load = true ) = 0;

		const std::vector< sVertex >&  getVertices() const { return m_vertices; }
		const std::vector< unsigned >& getIndices() const { return m_indices; }

		cUnique< cTexture2D > m_texture;
		cColor                m_color;

	protected:
		std::vector< sVertex >  m_vertices;
		std::vector< unsigned > m_indices;
	};
}
