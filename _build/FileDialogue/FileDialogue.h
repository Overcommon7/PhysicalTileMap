#pragma once
#include <vector>
#include <string>
#include <shtypes.h>

class FileBrowser
{
    std::vector<COMDLG_FILTERSPEC> m_filetypes;
    std::wstring m_startpath = L"";
    std::wstring m_saveExt = L"";

public:
    void PushFileType(const wchar_t* ext, const wchar_t* desc = L"");
    void PopFileType();
    void ResetFileTypes();
    void SetStartPath(const std::wstring& startPath);
    void SetDefaultExtension(const std::wstring& ext);
    void ClearStartPath();
    std::wstring GetFile() const;
    std::wstring SaveFile();
    std::wstring LoadFolderCondensed();
    std::wstring LoadFolder();
    FileBrowser() = default;
};
