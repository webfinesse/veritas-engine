#ifndef H_MATRIXSTACK
#define H_MATRIXSTACK

#include <memory>
#include "../VeritasEngineBase/MathTypes.h"

#include "SmallObject.h"


namespace VeritasEngine
{
	class MatrixStack: public SmallObject<>
	{
	public:
		MatrixStack();
		~MatrixStack();
		void Push(const Matrix4x4& matrix);
		const Matrix4x4& Peek() const;
		void Pop();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif