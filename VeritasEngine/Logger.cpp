#include "Logger.h"

#include <spdlog/spdlog.h>
#include <csignal>

struct VeritasEngine::Logger::Impl
{
	static void DumpLogs(int)
	{
		spdlog::get("Veritas Engine")->flush();
	}

	static void TerminateHandler()
	{
		DumpLogs(0);
	}

	Impl()
	{
		spdlog::set_async_mode(4096);
		m_logger = spdlog::daily_logger_mt("Veritas Engine", "VeritasEngineLog.txt");
		m_logger->set_level(spdlog::level::trace);

		signal(SIGABRT, DumpLogs);
		set_terminate(TerminateHandler);
	}

	~Impl()
	{
		spdlog::drop_all();
	}

	bool m_isEnabled{ false };
	std::shared_ptr<spdlog::logger> m_logger;
};

VeritasEngine::Logger::Logger()
	: m_impl{ std::make_unique<Impl>() }
{
	
}

VeritasEngine::Logger::Logger(Logger&& other) noexcept
	: m_impl{ std::move(other.m_impl) }
{
}

VeritasEngine::Logger::~Logger() noexcept = default;

void VeritasEngine::Logger::SetIsEnabled(bool enabled)
{
	m_impl->m_isEnabled = enabled;
}

bool VeritasEngine::Logger::IsEnabled() const
{
	return m_impl->m_isEnabled;
}

void VeritasEngine::Logger::Warn(const char* category, const char* message)
{
	if(m_impl->m_isEnabled)
	{
		m_impl->m_logger->warn("Category: {0}, Message: {1}", category, message);
	}
}

void VeritasEngine::Logger::Warn(const char* category, std::string& message)
{
	Warn(category, message.c_str());
}

void VeritasEngine::Logger::Trace(const char* category, const char* message)
{
	if (m_impl->m_isEnabled)
	{
		m_impl->m_logger->trace("Category: {0}, Message: {1}", category, message);
	}
}

void VeritasEngine::Logger::Trace(const char* category, std::string& message)
{
	Trace(category, message.c_str());
}

VeritasEngine::Logger& VeritasEngine::Logger::operator=(Logger&& other) noexcept
{
	if(&other != this)
	{
		m_impl = std::move(other.m_impl);
	}

	return *this;
}
