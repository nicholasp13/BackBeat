#pragma once

#include "bbpch.h"

#include "BackBeat/Events/KeyEvent.h"
#include "BackBeat/Events/MouseEvent.h"

// TODO: CREATE KEYCODES AND MOUSE CODES
namespace BackBeat {

	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);
		static MousePos GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};
}