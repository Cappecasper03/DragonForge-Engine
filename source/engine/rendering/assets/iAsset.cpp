#include "iAsset.h"

namespace df
{
	iAsset::iAsset( std::string _name )
		: name( std::move( _name ) )
	{}
}
