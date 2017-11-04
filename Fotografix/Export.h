#pragma once

class FGXImage;

namespace fgx {

struct ExportFormat
{
    const wchar_t* name;
    const wchar_t* extension;
    const wchar_t* mimeType;
    Gdiplus::PixelFormat pixelFormat;
    bool supportsQuality;
    bool supportsTransparency;
};

struct ExportOptions
{
    int quality;
    COLORREF backgroundColor;
};

enum class ExportResult
{
    Success,
    UnknownError,
    UnsupportedFormat,
    Win32Error
};

// Returns a list of supported export formats.
const std::vector<ExportFormat>& GetExportFormats();

// Exports an image to the specified file using the specified format.
ExportResult ExportImage(const FGXImage& image, const std::wstring& fileName, const ExportFormat& format, const ExportOptions& options);

}