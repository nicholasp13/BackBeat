#include "bbpch.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "BackBeat/Core/Application.h"
#include "BackBeat/Core/FileDialog.h"
namespace BackBeat {

    std::string FileDialog::OpenFile()
    {

        OPENFILENAMEA fileName;
        CHAR sizeFile[260] = { 0 };
        CHAR currentDir[256] = { 0 };
        ZeroMemory(&fileName, sizeof(OPENFILENAME));
        fileName.lStructSize = sizeof(OPENFILENAME);
        fileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
        fileName.lpstrFile = sizeFile;
        fileName.nMaxFile = sizeof(sizeFile);
        if (GetCurrentDirectoryA(256, currentDir))
            fileName.lpstrInitialDir = currentDir;
        fileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&fileName) == TRUE)
            return fileName.lpstrFile;

        return std::string();
    }

    void FileDialog::SaveFile() 
    { 
        // fileName.lpstrFilter = "WAV Files (*.wav)\0";  // Note: Code to filter what kind of files can be saved
    };
}




















    /**

#include <ShlObj.h>
#include <ShObjIdl_core.h>
#include <Shlwapi.h>
#include <winerror.h>

    const COMDLG_FILTERSPEC c_rgSaveTypes[] =
    {
        {L"Word Document (*.doc)",       L"*.doc"},
        {L"Web Page (*.htm; *.html)",    L"*.htm;*.html"},
        {L"Text Document (*.txt)",       L"*.txt"},
        {L"All Documents (*.*)",         L"*.*"}
    };


    class CDialogEventHandler : public IFileDialogEvents, public IFileDialogControlEvents
    {
    public:
        CDialogEventHandler() {};
        ~CDialogEventHandler() {};

        IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
        {
            static const QITAB qit[] = {
                QITABENT(CDialogEventHandler, IFileDialogEvents),
                QITABENT(CDialogEventHandler, IFileDialogControlEvents),
                { 0 },
    #pragma warning(suppress:4838)
            };
            return QISearch(this, qit, riid, ppv);
        }

        IFACEMETHODIMP_(ULONG) AddRef()
        {
            return InterlockedIncrement(&_cRef);
        }

        IFACEMETHODIMP_(ULONG) Release()
        {
            long cRef = InterlockedDecrement(&_cRef);
            if (!cRef)
                delete this;
            return cRef;
        }

        // IFileDialogEvents methods
        IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
        IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
        IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
        IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

        // IFileDialogControlEvents methods
        IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);
        IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
        IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
        IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

        HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv)
        {
            *ppv = NULL;
            CDialogEventHandler* pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
            HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
            if (SUCCEEDED(hr))
            {
                hr = pDialogEventHandler->QueryInterface(riid, ppv);
                pDialogEventHandler->Release();
            }
            return hr;

        }

    private:
        long _cRef;
    };

	class WindowsFileDialog : public FileDialog
	{
	public:

		static std::string OpenFile()
		{
			IFileDialog* fileDialog = nullptr;
            auto dialogEventHandler = CDialogEventHandler();
			HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&fileDialog));

			if (SUCCEEDED(hr))
            {
                // Create an event handling object, and hook it up to the dialog.
                IFileDialogEvents* pfde = NULL;
                hr = dialogEventHandler.CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
                if (SUCCEEDED(hr))
                {
                    // Hook up the event handler.
                    DWORD dwCookie;
                    hr = fileDialog->Advise(pfde, &dwCookie);
                    if (SUCCEEDED(hr))
                    {
                        // Set the options on the dialog.
                        DWORD dwFlags;

                        // Before setting, always get the options first in order 
                        // not to override existing options.
                        hr = fileDialog->GetOptions(&dwFlags);
                        if (SUCCEEDED(hr))
                        {
                            // In this case, get shell items only for file system items.
                            hr = fileDialog->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                            if (SUCCEEDED(hr))
                            {
                                // Set the file types to display only. 
                                // Notice that this is a 1-based array.
                                hr = fileDialog->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                                if (SUCCEEDED(hr))
                                {
                                    // Set the selected file type index to Word Docs for this example.
                                    hr = fileDialog->SetFileTypeIndex(1);
                                    if (SUCCEEDED(hr))
                                    {
                                        // Set the default extension to be ".doc" file.
                                        hr = fileDialog->SetDefaultExtension(L"doc;docx");
                                        if (SUCCEEDED(hr))
                                        {
                                            // Show the dialog
                                            hr = fileDialog->Show(NULL);
                                            if (SUCCEEDED(hr))
                                            {
                                                // Obtain the result once the user clicks 
                                                // the 'Open' button.
                                                // The result is an IShellItem object.
                                                IShellItem* psiResult;
                                                hr = fileDialog->GetResult(&psiResult);
                                                if (SUCCEEDED(hr))
                                                {
                                                    // We are just going to print out the 
                                                    // name of the file for sample sake.
                                                    PWSTR pszFilePath = NULL;
                                                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                                        &pszFilePath);
                                                    if (SUCCEEDED(hr))
                                                    {
                                                        TaskDialog(NULL,
                                                            NULL,
                                                            L"CommonFileDialogApp",
                                                            pszFilePath,
                                                            NULL,
                                                            TDCBF_OK_BUTTON,
                                                            TD_INFORMATION_ICON,
                                                            NULL);
                                                        CoTaskMemFree(pszFilePath);
                                                    }
                                                    psiResult->Release();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        // Unhook the event handler.
                        fileDialog->Unadvise(dwCookie);
                    }
                    pfde->Release();
                }
                fileDialog->Release();
            }
		}

		static void SaveFile() {
		
		}
	};
    /**/