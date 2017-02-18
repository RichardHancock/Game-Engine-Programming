#pragma once

#include <QrCode.hpp>
#include <SDL.h>

/** @brief	A qr code generator. */
class QRCode
{
public:

	/**
	 @brief	Constructor.

	 @param	inText				The text to encode into a QR Code.
	 @param	errorCorectLevel	The error correction level.
	 Adds extra data to the QR code to allow for slight errors in reading the code
	 */
	QRCode(std::string inText, const qrcodegen::QrCode::Ecc &errorCorrectionLevel);

	SDL_Surface* convertToSurface(int segmentSize);

protected:

	void printAllQRRects(SDL_Rect* rects, int arraySize);

	qrcodegen::QrCode qr;
};