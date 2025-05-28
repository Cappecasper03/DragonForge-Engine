#pragma once

#include <string>

#include "engine/core/utils/Misc.h"

namespace df
{
	class iTexture;

	class cTexture
	{
	public:
		DF_DeleteCopyAndMove( cTexture );

		cTexture( const std::string& _name );
		~cTexture();

		iTexture* operator->() const { return m_data; }
		iTexture& operator*() const { return *m_data; }

		iTexture* get() const { return m_data; }

	protected:
		iTexture* m_data;
	};
}
