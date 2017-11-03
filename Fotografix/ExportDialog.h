#pragma once

namespace fgx {

struct FileType
{
    std::wstring extension;
    std::wstring name;
};

class ExportDialog : private CFileDialog
{
public:
    ExportDialog(const std::vector<FileType>* fileTypes);
    virtual ~ExportDialog();

    bool Show();

    std::wstring GetFileName() const;
    const FileType& GetFileType() const;

private:
    const std::vector<FileType>* fileTypes_;
    IFileSaveDialog* comDialog_;
};

}