#pragma once

#include "Export.h"

namespace fgx {

// Encapsulates the File Export dialog.
class ExportDialog : private CFileDialog
{
public:
    ExportDialog();
    virtual ~ExportDialog();

    // Displays the dialog; returns false if the operation was cancelled.
    bool Show();

    std::wstring GetFileName() const;
    const ExportFormat& GetExportFormat() const;
    ExportOptions GetExportOptions() const;

private:
    void OnTypeChange();

private:
    const std::vector<ExportFormat>& formats_;
    IFileSaveDialog* comDialog_;
};

}