#pragma once

#include "bbpch.h"

#include "BackBeat/Events/KeyEvent.h"
#include "BackBeat/Events/MouseEvent.h"
#include "BackBeat/Core/KeyCodes.h"
#include "BackBeat/Core/MouseCodes.h"
// TODO: CREATE KEYCODES AND MOUSE CODES
namespace BackBeat {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static MousePos GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	};
}