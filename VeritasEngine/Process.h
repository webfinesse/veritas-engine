#ifndef H_PROCESS
#define H_PROCESS

#include <memory>
#include "DynamicLibraryHelper.h"
#include "../VeritasEngineBase/SmallObject.h"

using namespace std;

#pragma warning(push)
#pragma warning(disable: 4251)

namespace VeritasEngine
{
	class EXPORT Process : public SmallObject<>
	{
		friend class ProcessManager;
	public:
		enum class Status
		{
			Uninitalized,
			Running,
			Paused,
			Succeeded,
			Failed,
			Aborted
		};

		Process();
		virtual ~Process();

		Process::Status GetStatus() const;

		void Succeed();
		void Fail();
		void Pause();
		void UnPause();

	protected:
		virtual void OnInitialized();
		virtual void OnUpdate(float delta) = 0;
		virtual void OnSuccess();
		virtual void OnFail();
		virtual void OnAbort();

	private:
		struct Impl;
		unique_ptr<Impl> m_impl;
	};
}

#pragma warning(pop)

#endif