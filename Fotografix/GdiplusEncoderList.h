#pragma once

namespace fgx {

// Encapsulates the list of available GDI+ encoders.
class GdiplusEncoderList
{
public:
    GdiplusEncoderList();
    ~GdiplusEncoderList();

    const Gdiplus::ImageCodecInfo* begin() const;
    const Gdiplus::ImageCodecInfo* end() const;

private:
    Gdiplus::ImageCodecInfo* codecInfo_;
    unsigned int numCodecs_;
};

}