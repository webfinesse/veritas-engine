#ifndef H_ILOGGER
#define H_ILOGGER
#include "../VeritasEngineBase/StringHelper.h"


namespace VeritasEngine
{
	class ILogger
	{
	public:
		virtual ~ILogger() = default;

		virtual void SetIsEnabled(bool enabled) = 0;
		virtual bool IsEnabled() const = 0;

		virtual void Warn(const char* category, const char* message) = 0;

		template <typename ...Args>
		void Warn(const char* category, const char* messageFormat, Args... args)
		{
			auto const message = VeritasEngine::FormatString(std::forward<const char*>(messageFormat), std::forward<Args>(args)...);
			Warn(std::forward<const char*>(category), message.c_str());
		}

		virtual void Warn(const char* category, std::string& message) = 0;

		virtual void Trace(const char* category, const char* message) = 0;

		template <typename ...Args>
		void Trace(const char* category, const char* messageFormat, Args... args)
		{
			auto const message = VeritasEngine::FormatString(std::forward<const char*>(messageFormat), std::forward<Args>(args)...);
			Trace(std::forward<const char*>(category), message.c_str());
		}

		virtual void Trace(const char* category, std::string& message) = 0;
	};
}

#endif
