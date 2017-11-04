#include "stdafx.h"
#include "resource.h"
#include "ExportDialog.h"

#include <sstream>

namespace fgx {

static const int QUALITY_GROUP = 1;
static const int QUALITY_COMBO_BOX = 2;
static const int QUALITY_LOW = 3;
static const int QUALITY_MEDIUM = 4;
static const int QUALITY_HIGH = 5;

static std::wstring GenerateFilterString(const std::vector<ExportFormat>& formats)
{
    std::wostringstream filter;

    for (auto& format : formats) {
        filter << format.name << L" (*." << format.extension << L")|*." << format.extension << L"|";
    }

    filter << L"|";
    return filter.str();
}

static CString LoadCString(int id)
{
    CString str;
    str.LoadString(id);
    return str;
}

ExportDialog::ExportDialog()
    : CFileDialog{ false, nullptr, nullptr, OFN_OVERWRITEPROMPT, GenerateFilterString(GetExportFormats()).c_str() },
    formats_{ GetExportFormats() }
{
    ApplyOFNToShellDialog();
    comDialog_ = GetIFileSaveDialog();

    CString title = LoadCString(IDS_FILE_EXPORT);
    comDialog_->SetTitle(title);
    comDialog_->SetOkButtonLabel(title);

    StartVisualGroup(QUALITY_GROUP, LoadCString(IDS_EXPORT_QUALITY_LABEL));
    AddComboBox(QUALITY_COMBO_BOX);
    AddControlItem(QUALITY_COMBO_BOX, QUALITY_LOW, LoadCString(IDS_EXPORT_QUALITY_LOW));
    AddControlItem(QUALITY_COMBO_BOX, QUALITY_MEDIUM, LoadCString(IDS_EXPORT_QUALITY_MEDIUM));
    AddControlItem(QUALITY_COMBO_BOX, QUALITY_HIGH, LoadCString(IDS_EXPORT_QUALITY_HIGH));
    EndVisualGroup();

    SetSelectedControlItem(QUALITY_COMBO_BOX, QUALITY_MEDIUM);
    OnTypeChange();
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

    std::wstring expectedExt = GetExportFormat().extension;

    if (ext != expectedExt.c_str()) {
        path += L".";
        path += expectedExt.c_str();
    }

    return path;
}

const ExportFormat& ExportDialog::GetExportFormat() const
{
    UINT i = 1;
    comDialog_->GetFileTypeIndex(&i);
    return formats_[i - 1];
}

ExportOptions ExportDialog::GetExportOptions() const
{
    DWORD selectedQuality;
    const_cast<ExportDialog*>(this)->GetSelectedControlItem(QUALITY_COMBO_BOX, selectedQuality);

    ExportOptions options;

    switch (selectedQuality) {
        case QUALITY_LOW: options.quality = 25; break;
        case QUALITY_MEDIUM: options.quality = 75; break;
        case QUALITY_HIGH: options.quality = 100; break;
    }

    return options;
}

void ExportDialog::OnTypeChange()
{
    CDCONTROLSTATEF state = GetExportFormat().supportsQuality ? CDCS_ENABLEDVISIBLE : CDCS_INACTIVE;
    SetControlState(QUALITY_GROUP, state);
    SetControlState(QUALITY_COMBO_BOX, state);
}

}