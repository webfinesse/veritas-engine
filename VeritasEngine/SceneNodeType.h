#ifndef H_SCENENODETYPE
#define H_SCENENODETYPE

namespace VeritasEngine
{
	enum class SceneNodeType
	{
		None = 0,
		Camera = 1,
		Mesh = 2,
		Light = 3,
		ResourcedMesh = 4,
		AnimatedResourcedMesh = 5,
		End
	};
}

#endif