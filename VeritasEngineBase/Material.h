#ifndef H_MATERIAL
#define H_MATERIAL

#include "MathTypes.h"
#include "ResourceId.h"

#include "cereal/types/string.hpp"

namespace VeritasEngine
{
	struct Material
	{
		Float4 Ambient;
		Float4 Diffuse; 
		Float4 Specular;
		Float4 Emissive; 
		ResourceId DiffuseMap;
		ResourceId NormalMap;
		ResourceId SpecularMap;
		ResourceId ResourceId;

		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(Ambient, Diffuse, Specular, Emissive, DiffuseMap, NormalMap, SpecularMap);
		}
	};
}

#endif