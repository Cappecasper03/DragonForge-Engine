#include "iObject.h"

namespace df
{
	iObject::iObject( std::string _name )
		: m_name( std::move( _name ) )
	{}
}
