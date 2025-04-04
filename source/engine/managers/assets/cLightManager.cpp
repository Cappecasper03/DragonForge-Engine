#include "cLightManager.h"

namespace df
{
	cLightManager::cLightManager()
	{
		DF_ProfilingScopeCPU;

		create( "ambient", cLight::eType::eAmbient, 1.f );
	}
}
