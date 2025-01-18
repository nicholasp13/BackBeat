#include "bbpch.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "BackBeat/Core/Application.h"
#include "BackBeat/File/FileDialog.h"
namespace BackBeat {

	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAMEA fileName;
		CHAR sizeFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&fileName, sizeof(OPENFILENAME));
		fileName.lStructSize = sizeof(OPENFILENAME);
		fileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		fileName.lpstrFile = sizeFile;
		fileName.nMaxFile = sizeof(sizeFile);
		fileName.lpstrFilter = filter;
		if (GetCurrentDirectoryA(256, currentDir))
			fileName.lpstrInitialDir = currentDir;
		fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&fileName) == TRUE)
			return fileName.lpstrFile;

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA fileName;
		CHAR sizeFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&fileName, sizeof(OPENFILENAME));
		fileName.lStructSize = sizeof(OPENFILENAME);
		fileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		fileName.lpstrFile = sizeFile;
		fileName.nMaxFile = sizeof(sizeFile);
		fileName.lpstrFilter = filter;
		fileName.lpstrDefExt = strchr(filter, '\0') + 1;
		if (GetCurrentDirectoryA(256, currentDir))
			fileName.lpstrInitialDir = currentDir;
		fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetSaveFileNameA(&fileName) == TRUE)
			return fileName.lpstrFile;

		return std::string();
	}
}