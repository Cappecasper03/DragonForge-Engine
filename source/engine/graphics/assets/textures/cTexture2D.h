#pragma once

#include "engine/core/utils/Misc.h"
#include "iTexture.h"

namespace df
{
	class cTexture2D : public iTexture
	{
	public:
		DF_DeleteCopyAndMove( cTexture2D );

		cTexture2D( const std::string& _name );

		const cVector2u& getSize() const override { return m_size; }

		static cTexture2D* create( const std::string& _name );

	protected:
		std::string m_name;

		cVector2u m_size;
	};
}
