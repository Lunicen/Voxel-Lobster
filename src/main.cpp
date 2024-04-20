#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include "core/log.h"
#include "application/application.h"

void ParseArgument(const char *argument)
{
	if (strcmp(argument, "--trace") == 0)
	{
		Log::SetMode(Log::LogMode::trace);
	}

	if (strcmp(argument, "--debug") == 0)
	{
		Log::SetMode(Log::LogMode::debug);
	}
}

int main(const int argc, char *argv[])
{
	const auto& logger = Log::Get();
	for (auto i = 1; i < argc; ++i)
	{
		ParseArgument(argv[i]);
	}

	try
	{
		Application application("Settings.json");
		application.Run();
	}
	catch (const std::exception& e)
	{
		logger.Critical(e.what());
		return 1;
	}

	return 0;
}
