#include "iAsset.h"

namespace df
{
	iAsset::iAsset( std::string _name )
		: m_name( std::move( _name ) )
	{}
}
