#include "HumanInterfaceDevice.h"

int HumanInterfaceDevice::GetState(const KeyboardKey& key) const
{
	return glfwGetKey(_window.GetHandle(), static_cast<int>(key));
}

int HumanInterfaceDevice::GetState(const MouseButton& button) const
{
	return glfwGetMouseButton(_window.GetHandle(), static_cast<int>(button));
}

HumanInterfaceDevice::HumanInterfaceDevice(Window& window) : _window(window)
{
}

bool HumanInterfaceDevice::IsPressed(const KeyboardKey& key) const
{
	if (GetState(key) == GLFW_PRESS)
	{
		return true;
	}
	
	return false;
}

bool HumanInterfaceDevice::IsPressedOnce(const KeyboardKey& key)
{
	if (GetState(key) == GLFW_PRESS && _onceHandledKeyboardKeys.find(key) == _onceHandledKeyboardKeys.end())
	{
		_onceHandledKeyboardKeys.insert(key);
		return IsPressed(key);
	}

	if (GetState(key) == GLFW_RELEASE)
	{
		_onceHandledKeyboardKeys.erase(key);
	}

	return false;
}

bool HumanInterfaceDevice::IsPressed(const MouseButton& button) const
{
	if (GetState(button) == GLFW_PRESS)
	{
		return true;
	}

	return false;
}

bool HumanInterfaceDevice::IsPressedOnce(const MouseButton& button)
{
	if (GetState(button) == GLFW_PRESS && _onceHandledMouseButtons.find(button) == _onceHandledMouseButtons.end())
	{
		_onceHandledMouseButtons.insert(button);
		return IsPressed(button);
	}

	if (GetState(button) == GLFW_RELEASE)
	{
		_onceHandledMouseButtons.erase(button);
	}

	return false;
}

std::pair<double, double> HumanInterfaceDevice::GetCursorPosition() const
{
	double x;
	double y;
	glfwGetCursorPos(_window.GetHandle(), &x, &y);

	return {x, y};
}

void HumanInterfaceDevice::SetCursorPosition(const double x, const double y) const
{
	glfwSetCursorPos(_window.GetHandle(), x, y);
}

void HumanInterfaceDevice::EnableCursor() const
{
	glfwSetInputMode(_window.GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void HumanInterfaceDevice::DisableCursor() const
{
	glfwSetInputMode(_window.GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
