#pragma once
// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>

#define GLT_IMPLEMENTATION

#pragma warning(push, 0)
#include "gltext.h"
#pragma warning(pop)

/// @class FPSCounter
/// @brief Calculates and shows actual value of FPS.
class FPSCounter
{
	GLTtext* _fps;
	double _lastTime;
	int _numberOfFrames;
	int _actualFps;

	void CountFps();

public:

	/// @brief The constructor.
	FPSCounter();

	FPSCounter(const FPSCounter&) = delete;
	FPSCounter(FPSCounter&&) = delete;
	FPSCounter& operator=(const FPSCounter&) = delete;
	FPSCounter& operator=(FPSCounter&&) = delete;

	/// @brief Updates actual value of FPS and shows it on the screen.
	void Update();

	~FPSCounter();
	
};
