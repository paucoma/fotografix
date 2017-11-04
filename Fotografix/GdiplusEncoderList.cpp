#include "stdafx.h"
#include "GdiplusEncoderList.h"

namespace fgx {

GdiplusEncoderList::GdiplusEncoderList() : codecInfo_{ nullptr }, numCodecs_{ 0 }
{
    UINT size = 0;

    if (Gdiplus::GetImageEncodersSize(&numCodecs_, &size) != Gdiplus::Status::Ok) {
        return;
    }

    codecInfo_ = static_cast<Gdiplus::ImageCodecInfo*>(malloc(size));

    if (codecInfo_ != nullptr) {
        Gdiplus::GetImageEncoders(numCodecs_, size, codecInfo_);
    }
}

GdiplusEncoderList::~GdiplusEncoderList()
{
    free(codecInfo_);
}

const Gdiplus::ImageCodecInfo* GdiplusEncoderList::begin() const
{
    return codecInfo_;
}

const Gdiplus::ImageCodecInfo* GdiplusEncoderList::end() const
{
    return codecInfo_ + numCodecs_;
}

}