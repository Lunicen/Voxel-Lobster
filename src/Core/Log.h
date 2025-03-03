#pragma once
#include "spdlog/spdlog.h"



/// @class   Log
/// @brief   Logs the occurred events with additional information and status.
/// @details This static class is made for logging events where actions might contain important messages.
/// Used for tracking control flow, errors, etc.
///	If you need more information about the log levels and when to use them,
///	check out [this](https://stackoverflow.com/questions/2031163/when-to-use-the-different-log-levels) link.
class Log
{
	Log() {
		spdlog::set_pattern("[%T] [%^%l%$] %v");
		spdlog::set_level(spdlog::level::info);
		spdlog::info("Logging initialized!");
	}

public:

	/// @brief Represents the levels of detail logging information to the console.
	enum class LogMode
	{
		trace = 0,
		debug,
		normal
	};

	Log(const Log&) = delete;
	Log(Log&&) = delete;
	Log& operator=(Log) = delete;
	Log& operator=(Log&&) = delete;

	/// @brief Returns Logger instance
	static Log& Get()
	{
		static Log instance;
		return instance;
	}

	/// @brief Sets the log mode.
	///	@param mode - chosen logging mode.
	///	@see LogMode
	static void SetMode(LogMode mode);

	/// @brief Prints tracing message.
	///	@details The trace is used to print EVERYTHING that is happening.
	///	@warning Never, ever use it in production! Only during the development process.
	/// @param message - Information about the event.
	void Trace(const std::string& message) const;

	/// @brief Prints debug message.
	///	@details The debug is used to print the information for diagnostics.
	/// @param message - Information about the event.
	void Debug(const std::string& message) const;

	/// @brief Prints info message.
	///	@details The info is used to print the general information about occurred events.
	/// @param message - Information about the event.
	void Info(const std::string& message) const;

	/// @brief Prints warning message.
	///	@details The warning is used to warn that the occurred event might cause oddities in the future.
	/// @param message - Information about the event.
	void Warn(const std::string& message) const;

	/// @brief Prints error message.
	///	@details The error is used to signalize incorrect behaviour that can be handled (for ex. missing data). 
	/// @param message - Information about the event.
	void Error(const std::string& message) const;

	/// @brief Prints critical message.
	///	@details The critical is used to print the error that caused a force shutdown of the application.
	/// @param message - Information about the event.
	void Critical(const std::string& message) const;

	~Log() = default;
};