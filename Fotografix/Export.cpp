#include "stdafx.h"
#include "Export.h"

#include "GdiplusEncoderList.h"
#include "FGXImage.h"

namespace fgx {

static std::vector<ExportFormat> exportFormats = {
    ExportFormat{ L"JPEG", L"jpg", L"image/jpeg", PixelFormat24bppRGB,  true,  false },
    ExportFormat{ L"PNG",  L"png", L"image/png",  PixelFormat32bppARGB, false, true },
    ExportFormat{ L"GIF",  L"gif", L"image/gif",  PixelFormat32bppARGB, false, true },
    ExportFormat{ L"TIFF", L"tif", L"image/tiff", PixelFormat32bppARGB, false, true }
};

static std::wstring GetFileExtension(const std::wstring& fileName)
{
    size_t pos = fileName.find_last_of(L'.');
    return pos == std::wstring::npos ? std::wstring{} : fileName.substr(pos + 1);
}

static bool GetEncoderClsid(const std::wstring& mimeType, CLSID& clsid)
{
    GdiplusEncoderList encoderList;

    for (auto& encoder : encoderList) {
        if (mimeType == encoder.MimeType) {
            clsid = encoder.Clsid;
            return true;
        }
    }

    return false;
}

static void CopyLayerToBitmap(const FGXLayer& layer, Gdiplus::Bitmap& bitmap)
{
    Rect rect(0, 0, bitmap.GetWidth(), bitmap.GetHeight());

    auto pixels = std::unique_ptr<DWORD[]>{ new DWORD[rect.Width * rect.Height] };
    layer.SaveToMemory(pixels.get(), 4);

    BitmapData data;
    data.Width = rect.Width;
    data.Height = rect.Height;
    data.Stride = rect.Width * 4;
    data.PixelFormat = PixelFormat32bppARGB;
    data.Scan0 = pixels.get();
    data.Reserved = NULL;

    bitmap.LockBits(&rect, ImageLockMode::ImageLockModeWrite | ImageLockMode::ImageLockModeUserInputBuf, data.PixelFormat, &data);
    bitmap.UnlockBits(&data);
}

const std::vector<ExportFormat>& GetExportFormats()
{
    return exportFormats;
}

ExportResult ExportImage(const FGXImage& image, const std::wstring& fileName, const ExportFormat& format, const ExportOptions& options)
{
    CLSID clsid;
    if (!GetEncoderClsid(format.mimeType, clsid)) {
        return ExportResult::UnsupportedFormat;
    }

    int width = image.GetWidth();
    int height = image.GetHeight();

    CRect bounds{ 0, 0, width, height };
    FGXLayer layer{ bounds };

    if (format.supportsTransparency) {
        image.Render(layer, bounds, true);
    } else {
        layer.Fill(FGXColor{ options.backgroundColor, 255 });
        image.Render(layer, bounds, false);
    }

    Bitmap bitmap(width, height, format.pixelFormat);
    CopyLayerToBitmap(layer, bitmap);

    Gdiplus::Status status;

    if (format.supportsQuality) {
        EncoderParameters params;

        params.Count = 1;
        params.Parameter[0].Guid = Gdiplus::EncoderQuality;
        params.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
        params.Parameter[0].NumberOfValues = 1;
        params.Parameter[0].Value = const_cast<int*>(&options.quality);

        status = bitmap.Save(fileName.c_str(), &clsid, &params);
    } else {
        status = bitmap.Save(fileName.c_str(), &clsid);
    }

    switch (status) {
        case Gdiplus::Status::Ok:
            return ExportResult::Success;

        case Gdiplus::Status::OutOfMemory:
            SetLastError(ERROR_OUTOFMEMORY);
            return ExportResult::Win32Error;

        case Gdiplus::Status::AccessDenied:
            SetLastError(ERROR_ACCESS_DENIED);
            return ExportResult::Win32Error;

        case Gdiplus::Status::Win32Error:
            return ExportResult::Win32Error;

        case Gdiplus::Status::UnknownImageFormat:
            return ExportResult::UnsupportedFormat;

        default:
            return ExportResult::UnknownError;
    }
}

}