#include "bbpch.h"

#include "BackBeat/Core/Input.h"

#include "BackBeat/Core/Application.h"
#include <GLFW/glfw3.h>

// TODO: Create own codes to replace int
namespace BackBeat {
	
	bool Input::IsKeyPressed(int key)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, key);
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	MousePos Input::GetMousePosition() { 
		auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return MousePos(x, y); }

	float Input::GetMouseX() { return GetMousePosition().x; }

	float Input::GetMouseY() { return GetMousePosition().y; }
}