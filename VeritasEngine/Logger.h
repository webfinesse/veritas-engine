#ifndef H_LOGGER
#define H_LOGGER

#include "ILogger.h"

#include <memory>
#include "SmallObject.h"

namespace VeritasEngine
{
	class Logger : public SmallObject<>, public ILogger
	{
	public:
		Logger();
		Logger(const Logger&) = delete;
		Logger(Logger&& other) noexcept;

		~Logger() noexcept override;

		void SetIsEnabled(bool enabled) override;
		bool IsEnabled() const override;

		void Warn(const char* category, const char* message) override;
		void Warn(const char* category, std::string& message) override;

		void Trace(const char* category, const char* message) override;
		void Trace(const char* category, std::string& message) override;

		Logger& operator=(Logger&& other) noexcept;
		Logger& operator=(const Logger&) = delete;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
