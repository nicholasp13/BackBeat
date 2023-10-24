#include "bbpch.h"

#include <GLFW/glfw3.h>

#include "BackBeat/Core/Application.h"
#include "BackBeat/Core/Input.h"
namespace BackBeat {
	
	bool Input::IsKeyPressed(KeyCode key)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, key);
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	MousePos Input::GetMousePosition() { 
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return MousePos((float)x, (float)y); }

	float Input::GetMouseX() { return GetMousePosition().x; }

	float Input::GetMouseY() { return GetMousePosition().y; }
}