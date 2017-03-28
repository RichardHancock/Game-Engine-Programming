#pragma once

#include <QrCode.hpp>
#include <SDL.h>

/** @brief	A QR Code generator. */
class QRCode
{
public:

	/**
	 @brief	Constructor.

	 @param	inText				The text to encode into a QR Code.
	 @param	errorCorrectionLevel	The error correction level. Follows the QR standards:
	 http://www.qrcode.com/en/about/error_correction.html
	 Adds extra data to the QR code to allow for slight errors in reading the code
	 */
	QRCode(std::string inText, const qrcodegen::QrCode::Ecc &errorCorrectionLevel);

	/**
	 @brief	Converts the QR Code to a SDL_Surface.
	 Adds a 1 segmentSize white border around the QR Code to improve the decoding by cameras.
	
	 @param	segmentSize	Size of each segment(pixel) of the QR code. 
	 Increase this number to scale the QR code surface
	
	 @return	Null if it fails, else the QR Code converted to a surface.
	 */
	SDL_Surface* convertToSurface(int segmentSize);

protected:

	/**
	 @brief	Print all QR rectangles.
	 Debug function to test the convertToSurface function
	
	 @param [in]	rects	 	The array of rectangles.
	 @param 		arraySize	Size of the array.
	 */
	void printAllQRRects(SDL_Rect* rects, int arraySize);

	/** @brief	The QR Code Data. */
	qrcodegen::QrCode qr;
};