#include "Application.h"

#include "Core/EngineExceptions.h"
#include "LayerStack/Stack/SandboxStack.h"

// As a static member of class this variable
// must be here initialized
Window Application::_window{};

void Application::WindowResizeEvent(GLFWwindow* const, const int width, const int height)
{
	glViewport(0, 0, width, height);

	_window.SetWidth(static_cast<size_t>(width));
	_window.SetHeight(static_cast<size_t>(height));
}


void Application::CentralizeWindow() const
{
	const auto& x = _fullscreenWidth / 2 - _window.GetWidth() / 2;
	const auto& y = _fullscreenHeight / 2 - _window.GetHeight() / 2;

	glfwSetWindowPos(_window.GetHandle(), static_cast<int>(x), static_cast<int>(y));
}

void Application::Initialize()
{
	glfwInit();

	constexpr auto versionMajor = 3;
	constexpr auto versionMinor = 3;

	// The target version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const auto& width = static_cast<int>(_window.GetWidth());
	const auto& height = static_cast<int>(_window.GetHeight());

	_window.SetHandle(glfwCreateWindow(width, height, "Block Forge", nullptr, nullptr));
	if (_window.GetHandle() == nullptr)
	{
		glfwTerminate();
		throw BadInitializationException("Failed to create window.");
	}

	glfwMakeContextCurrent(_window.GetHandle());
	gladLoadGL();

	glfwSetWindowUserPointer(_window.GetHandle(), this);

	glViewport(0, 0, width, height);

	const auto screenMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	_fullscreenWidth = screenMode->width;
	_fullscreenHeight = screenMode->height;

	if (_fullscreenWidth < static_cast<size_t>(width) || _fullscreenHeight < static_cast<size_t>(height))
	{
		_log.Warn("The resolution size settings " + 
			std::to_string(width) + "x" + std::to_string(height) + 
			" exceed the supported monitor resolution " + 
			std::to_string(_fullscreenWidth) + "x" + std::to_string(_fullscreenHeight) +
			"."
		);
	}

	SetCallbacks();

	CentralizeWindow();

	_log.Info("Block Forge initialized!");
}

void Application::SetCallbacks()
{
	glfwSetFramebufferSizeCallback(_window.GetHandle(), WindowResizeEvent);
}

Application::Application(const std::string& filenameWithSettings)
{
	_settings.Load(filenameWithSettings);

	const auto resolution = _settings.GetJsonObject("resolution");
	_window.SetHeight(resolution["height"].get<int>());
	_window.SetWidth(resolution["width"].get<int>());
}

void Application::Run()
{
	Initialize();

	const auto sandbox = SandboxStack(_window);
	_hid.DisableCursor();

	while(!glfwWindowShouldClose(_window.GetHandle()))
	{
		glfwPollEvents();

		if (_window.GetWidth() <= 0 || _window.GetHeight() <= 0) continue;

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sandbox.Update();
		sandbox.ProcessEvents(_hid);

		glfwSwapBuffers(_window.GetHandle());
	}

	_log.Info("Quitting...");

	glfwDestroyWindow(_window.GetHandle());
	glfwTerminate();
}
