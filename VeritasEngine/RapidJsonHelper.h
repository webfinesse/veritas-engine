#ifndef H_RAPIDJSONHELPER
#define H_RAPIDJSONHELPER

#include "..\Includes\rapidjson\include\rapidjson\document.h"

namespace VeritasEngine
{
	using JsonValue = const rapidjson::GenericValue < rapidjson::UTF8<> > ;
}

#endif