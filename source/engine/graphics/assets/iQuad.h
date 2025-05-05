#pragma once

#include <vector>

#include "engine/core/math/cVector.h"
#include "engine/misc/cColor.h"
#include "iRenderAsset.h"

namespace df
{
	class iTexture;

	class iQuad : public iRenderAsset
	{
	public:
		DF_DisableCopyAndMove( iQuad );

		struct sVertex
		{
			cVector3f position = cVector3f( 0 );
			cVector2f tex_coord;
		};

		iQuad( std::string _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color = color::white );
		~iQuad() override;

		virtual bool loadTexture( const std::string& _file_path, bool _mipmapped = true, int _mipmaps = 0, bool _flip_vertically_on_load = true ) = 0;

		const std::vector< sVertex >&  getVertices() const { return m_vertices; }
		const std::vector< unsigned >& getIndices() const { return m_indices; }

		iTexture* texture;
		cColor    color;

	protected:
		std::vector< sVertex >  m_vertices;
		std::vector< unsigned > m_indices;
	};
}
