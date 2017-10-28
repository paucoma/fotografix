#pragma once

#include <cmath>
#include <utility>

#define ForeachPixel(c) for (BYTE *p = (c).data, *_p = (c).data + position.Width() * position.Height(); p < _p; p++)
#define ForeachPixel2(c1, c2) for (BYTE *p = (c1).data, *_p = (c1).data + position.Width() * position.Height(), *o = (c2).data; p < _p; p++, o++)
//#define ForeachRectPixel for (int j = common.top; j < common.bottom; j++) for (int i = common.left; i < common.right; i++)
#define ForeachPixelRGB for (BYTE *r = channels[1].data, *g = channels[2].data, *b = channels[3].data, *_r = channels[1].data + position.Width() * position.Height(); r < _r; r++, g++, b++)

#define BeginLoopRect(rect, perPixel) \
	for (int j = rect.top; j < rect.bottom; j++) {\
		for (int i = rect.left; i < rect.right; i++, perPixel)

#define EndLoopRect(perLine) \
		perLine;\
	}

using std::swap;

template <class T>
__forceinline T clamp(T value, T min, T max) {
	return value < min ? min : (value > max ? max : value);
}

inline BYTE FGXMultiply(BYTE value1, BYTE value2) {
	return (WORD(value1) * WORD(value2)) / 255;
}

inline BYTE FGXDivide(BYTE value1, BYTE value2) {
	return (WORD(value1) * 255) / WORD(value2);
}

inline BYTE FGXScreen(BYTE value1, BYTE value2) {
	return 255 - WORD(255 - value1) * WORD(255 - value2) / 255;
}

inline BYTE FGXScreen2(BYTE value1, BYTE value2) {
	return 255 - 2 * WORD(255 - value1) * WORD(255 - value2) / 255;
}

inline BYTE FGXOverlay(BYTE value1, BYTE value2) {
	return (value2 & 0x80) == 0 ? (FGXMultiply(value1, value2) << 1) : FGXScreen2(value1, value2);
}

inline BYTE FGXBlend(BYTE src, BYTE srcAlpha, BYTE dest) {
	return (WORD(src) * WORD(srcAlpha) + WORD(dest) * WORD(255 - srcAlpha)) / 255;
}

inline BYTE FGXAlphaBlend(BYTE src, BYTE srcAlpha, BYTE dest, BYTE destAlpha) {
	register BYTE a = FGXScreen(srcAlpha, destAlpha);
	return a == 0 ? 0 : (WORD(src) * WORD(srcAlpha) + WORD(dest) * (WORD(destAlpha) * WORD(255 - srcAlpha) / 255)) / a;
}

inline BYTE FGXInvert(BYTE value) {
	return 255 - value;
}

inline BYTE FGXBrightnessPlus(BYTE value, BYTE d) {
	WORD result = WORD(value) + WORD(d);
	return result > 255 ? 255 : result;
}

inline BYTE FGXBrightnessMinus(BYTE value, BYTE d) {
	return (value < d) ? 0 : (value - d);
}

inline BYTE FGXContrastPlus(BYTE value, BYTE d) {
	short r = (short(-128) * d + short(value) * (255 + d)) / 255;
	return clamp<short>(r, 0, 255);
}

inline BYTE FGXContrastMinus(BYTE value, BYTE d) {
	return (WORD(128) * d + WORD(value) * (255 - d)) / 255;
}

inline BYTE FGXDifference(BYTE value1, BYTE value2) {
	return value1 < value2 ? value2 - value1 : value1 - value2;
}

inline BYTE FGXExclusion(BYTE value1, BYTE value2) {
	return value1 ^ value2;
}

inline BYTE FGXDarken(BYTE value1, BYTE value2) {
	return value1 < value2 ? value1 : value2;
}

inline BYTE FGXLighten(BYTE value1, BYTE value2) {
	return value1 < value2 ? value2 : value1;
}

inline BYTE FGXPinLight(BYTE value1, BYTE value2) {
	if ((value1 & 0x80) == 0)
		return value1 < value2 ? value1 : value2;
	else
		return value1 < value2 ? value2 : value1;
}

inline BYTE FGXHardMix(BYTE value1, BYTE value2) {
	return (WORD(value1) + value2 > 255) ? 255 : 0;
}

inline BYTE FGXLevels(BYTE value, BYTE s, BYTE h) {
	return clamp<short>((value - s) * 255 / (h - s), 0, 255);
}

inline BYTE FGXGamma(BYTE value, short gamma) {
	return clamp<short>(pow(value, gamma / 100.0f), 0, 255);
}

inline BYTE FGXPosterize(BYTE value, BYTE level) {
	return value & ((255 >> level) << level);
}

inline BYTE FGXGrayscale(BYTE r, BYTE g, BYTE b) {
	return ((WORD)r + (WORD)g + (WORD)b) / 3;
}

inline BYTE FGXGrayscaleEx(BYTE r, BYTE g, BYTE b, short rp, short gp, short bp) {
	register short p = rp + gp + bp;
	return p == 0 ? 0 : (short(r) * rp + short(g) * gp + short(b) * bp) / p;
}

inline BYTE FGXSaturation(BYTE r, BYTE g, BYTE b) {
	return max(max(r, g), b) - min(min(r, g), b);
}

inline BYTE FGXEmboss(BYTE c1, BYTE c2) {
	return c1 + 128 - c2;
}

inline int bswap(int i) {
	return _byteswap_ulong(i);
}

inline unsigned short bswap(unsigned short i) {
	return _byteswap_ushort(i);
}

inline float noise(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	return 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

inline float smooth_noise(int x, int y) {
	return (noise(x-1, y-1) + noise(x+1, y-1) + noise(x-1, y+1) + noise(x+1, y+1)) / 16
		 + (noise(x-1, y) + noise(x+1, y) + noise(x, y-1) + noise(x, y+1)) / 8
		 + noise(x, y) / 4;
}

inline float interpolate(float a, float b, float x) {
	return a * x + b * (1.0 - x);
}

inline float interpolated_noise(float x, float y) {
	int ix = x,
		iy = y;
	
	float fx = x - ix,
		  fy = y - iy;

	float v1 = smooth_noise(ix, iy),
		  v2 = smooth_noise(ix+1, iy),
		  v3 = smooth_noise(ix, iy+1),
		  v4 = smooth_noise(ix+1, iy+1);

	float i1 = interpolate(v1, v2, fx),
		  i2 = interpolate(v3, v4, fx);

	return interpolate(i1, i2, fy);
}

inline BYTE FGXPerlin(int x, int y, float p, int n) {
	float total = 0.0;

	for (int i = 0; i < n; i++) {
		float freq = 1 << i,
			  amp = pow(p, i);

		total += interpolated_noise(x * freq, y * freq) * amp;
	}

	return total * 255.0;
}

inline BYTE interpolate_color(float x1, BYTE c1, float x2, BYTE c2, float x) {
	return (x - x1) * c2 / (x2 - x1) + (x2 - x) * c1 / (x2 - x1);
}
