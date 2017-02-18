#include "QRCode.h"

#include <assert.h>
#include <iostream>

#include "../misc/Log.h"

QRCode::QRCode(std::string inText, const qrcodegen::QrCode::Ecc& errorCorrectionLevel)
	: qr(qrcodegen::QrCode::encodeText(inText.c_str(), errorCorrectionLevel))
{
	if (inText.length() > 500)
	{
		//This should be fine as I foresee no scenario where room codes need to get anywhere close to this length
		Log::logW("These functions have only been tested using strings of 500 chars or less.");
		Log::logW("Note from qrcodegen lib docs: this function is guaranteed to succeed for strings that have 738 or fewer Unicode code points(not UTF - 16 code units)");
	}
}

SDL_Surface* QRCode::convertToSurface(int segmentSize)
{
	if (segmentSize < 1)
	{
		Log::logE("Segment size passed to qrToSurface() is below 1.");
		return nullptr;
	}
	
	int size = qr.size;
	SDL_Rect* rects = new SDL_Rect[size*size];

	int arrayIt = 0; //Get this working with just X and Y

	for (int y = 0; y < size; y++)
	{
		int yCoord = y * segmentSize;

		for (int x = 0; x < size; x++)
		{

			if (qr.getModule(x, y) == 1)
			{
				SDL_Rect rect;
				rect.h = segmentSize;
				rect.w = segmentSize;
				rect.x = (x * segmentSize);
				rect.y = yCoord;

				rects[arrayIt] = rect;
			}
			else
			{
				//Bad Hack
				SDL_Rect rect;
				rect.h = 0;
				rect.w = 0;
				rect.x = 0;
				rect.y = 0;

				rects[arrayIt] = rect;
			}

			arrayIt++;
		}
	}
	//printAllQRRects(rects, size * size);

	SDL_Surface* surface;
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	int dimensions = size * segmentSize;

	surface = SDL_CreateRGBSurface(0, dimensions, dimensions, 32,
		rmask, gmask, bmask, amask);

	if (surface == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
	SDL_FillRects(surface, rects, size * size, SDL_MapRGB(surface->format, 0, 0, 0));


	delete[] rects;
	return surface;
}

void QRCode::printAllQRRects(SDL_Rect * rects, int arraySize)
{
	for (int i = 0; i < arraySize; i++)
	{
		SDL_Rect rect = rects[i];
		std::cout << "Rect " << i << ": x: " << rect.x << " y: " << rect.y << " w: " << rect.w << " h: " << rect.h << std::endl;
	}
}
