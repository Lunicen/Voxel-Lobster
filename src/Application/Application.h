#pragma once
#include "Core/Log.h"
#include "Core/Metadata.h"
#include "Window.h"
#include "HID/HumanInterfaceDevice.h"

/// @class Application
///	@brief Represents the application context.
/// @details Application manages events that are bound to the logic of an application. 
/// For example, handling window events such as resize, minimize, exit, etc... 
/// and providing a control flow for selecting what to display in the application.
///	@note This class should be called at the entry point.
class Application
{
	static Window _window;
	HumanInterfaceDevice _hid{_window};

	Log& _log = Log::Get();

	Metadata _settings;

	size_t _fullscreenWidth{};
	size_t _fullscreenHeight{};

	static void WindowResizeEvent(GLFWwindow*, int width, int height);
	
	void CentralizeWindow() const;
	void Initialize();
	static void SetCallbacks();

public:

	/// @brief The constructor.
	///	@param filenameWithSettings - path to the JSON file with application settings.
	explicit Application(const std::string& filenameWithSettings);

	/// @brief Creates an application and runs it.
	void Run();
};

