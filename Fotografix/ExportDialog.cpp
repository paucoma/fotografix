#include "stdafx.h"
#include "resource.h"
#include "ExportDialog.h"

#include <sstream>

namespace fgx {

static std::wstring GenerateFilterString(const std::vector<FileType>& fileTypes)
{
    std::wostringstream filter;

    for (auto& type : fileTypes) {
        filter << type.name << L" (*." << type.extension << L")|*." << type.extension << L"|";
    }

    filter << L"|";
    return filter.str();
}

ExportDialog::ExportDialog(const std::vector<FileType>* fileTypes)
    : CFileDialog{ false, nullptr, nullptr, OFN_OVERWRITEPROMPT, GenerateFilterString(*fileTypes).c_str() },
    fileTypes_{ fileTypes }
{
    comDialog_ = GetIFileSaveDialog();

    CString title;
    title.LoadString(IDS_FILE_EXPORT);
    comDialog_->SetTitle(title);
}

ExportDialog::~ExportDialog()
{
    comDialog_->Release();
}

bool ExportDialog::Show()
{
    return DoModal() == IDOK;
}

std::wstring ExportDialog::GetFileName() const
{
    CString path = GetPathName();
    CString ext = GetFileExt();

    std::wstring expectedExt = GetFileType().extension;

    if (ext != expectedExt.c_str()) {
        path += L".";
        path += expectedExt.c_str();
    }

    return path;
}

const FileType& ExportDialog::GetFileType() const
{
    UINT i = 1;
    comDialog_->GetFileTypeIndex(&i);
    return fileTypes_->at(i - 1);
}

}