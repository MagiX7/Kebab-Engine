#include "Application.h"
#include "Window.h"
#include "FileDialog.h"

#include "GL/glew.h"
#include "GL/wglew.h"

#include <Windows.h>
#include <commdlg.h>

std::string FileDialog::OpenFile(const char* filter)
{
	OPENFILENAME file;
	char size[260] = {};
	ZeroMemory(&file, sizeof(OPENFILENAME));
	file.lStructSize = sizeof(OPENFILENAME);

	file.hwndOwner = GetActiveWindow();
	file.lpstrFile = size;
	file.nMaxFile = sizeof(file);
	file.lpstrFilter = filter;
	file.nFilterIndex = 1;
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileName(&file))
	{
		return file.lpstrFile;
	}

	return "Couldn't open file dialog";
}

std::string FileDialog::SaveFile(const char* filter)
{
	OPENFILENAME file;
	char size[260] = {};
	ZeroMemory(&file, sizeof(OPENFILENAME));
	file.lStructSize = sizeof(OPENFILENAME);

	file.hwndOwner = GetActiveWindow();
	file.lpstrFile = size;
	file.nMaxFile = sizeof(file);
	file.lpstrFilter = filter;
	file.nFilterIndex = 1;
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileName(&file))
	{
		return file.lpstrFile;
	}

	return "Couldn't save file dialog";
}