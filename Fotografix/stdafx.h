// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4482)
#pragma warning(disable : 4800)
#pragma warning(disable : 4805)
#pragma warning(disable : 4996)


#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#define WINVER _WIN32_WINNT_WIN7
#define _WIN32_WINNT _WIN32_WINNT_WIN7

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxadv.h>







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#include <gdiplus.h>

using Gdiplus::GdiplusStartup;
using Gdiplus::GdiplusShutdown;
using Gdiplus::GdiplusStartupInput;

using Gdiplus::Rect;
using Gdiplus::RectF;
using Gdiplus::Point;
using Gdiplus::PointF;
using Gdiplus::Status;

using Gdiplus::Graphics;
using Gdiplus::Bitmap;
using Gdiplus::BitmapData;
using Gdiplus::PixelFormat;
using Gdiplus::ImageLockMode;
using Gdiplus::EncoderParameters;
using Gdiplus::ColorPalette;
using Gdiplus::ARGB;

using Gdiplus::Font;
using Gdiplus::FontStyle;

#include <string>
#include <vector>
#include <memory>
