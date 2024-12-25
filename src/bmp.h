
#ifndef __BMP_H__
#define __BMP_H__

#include "common.h"

#include <fstream>

#pragma pack(1)

struct Filehder
{
    /*
        bfType (2 bytes): Indicates the file type, with a fixed value of 0x4D42, corresponding to the ASCII characters BM, which signify that this is a BMP file.
	•	bfSize (4 bytes): The size of the entire file, in bytes.
	•	bfReserved1 (2 bytes): Reserved field, must be 0.
	•	bfReserved2 (2 bytes): Reserved field, must be 0.
	•	bfOffBits (4 bytes): The offset from the beginning of the file header to the actual image data (i.e., the starting position of the image data).
    */
public:
    ub2 Type;
    ub4 Size;
    ub2 Reserved1;
    ub2 Reserved2;
    ub4 OffBits;
};

struct Infohder
{
    /*
    •	biSize (4 bytes): The size of this header, typically 40 bytes.
	•	biWidth (4 bytes): The width of the image in pixels.
	•	biHeight (4 bytes): The height of the image in pixels. Note that BMP image data is stored from bottom to top, so when the height is positive, the image is stored starting from the bottom.
	•	biPlanes (2 bytes): The number of color planes, must be 1.
	•	biBitCount (2 bytes): The number of bits per pixel. For a 24-bit BMP, this value is 24.
	•	biCompression (4 bytes): The compression method used for the image. A value of 0 indicates no compression.
	•	biSizeImage (4 bytes): The size of the image data. If there is no compression, this value can be set to 0.
	•	biXPelsPerMeter (4 bytes): The horizontal resolution, in pixels per meter.
	•	biYPelsPerMeter (4 bytes): The vertical resolution, in pixels per meter.
	•	biClrUsed (4 bytes): The number of colors used in the color table. For a 24-bit BMP, this is usually 0, indicating that all colors are used.
	•	biClrImportant (4 bytes): The number of important colors. Typically set to 0, meaning all colors are considered important.
    */
public:
    ub4 InfohderSize; // 这里需要check一下是不是四十字节
    ub4 Width;
    ub4 Height;
    ub2 BiPlanes; // check
    ub2 BitCount;
    ub4 Comression; // check
    ub4 Sizeimage;
    ub4 WidthPixelsPerMeter;
    ub4 HeightPixelsPerMeter;
    ub4 BiClrUsed;
    ub4 BiClrImportant;
};

#pragma pack()

class Bmp
{
public:
    ub8 HideScale;

    bool Load(std::string Path);
    bool Save(std::string Path);
    bool process();
    bool extract();

    bool GetMessage();

    ub8 LineByteCalculator(ub4 Width);
    std::string ctostr(char c);
    ub8 hidetomap(ub8 x);

private:
    Filehder Fhd;
    Infohder Ihd;
    ub1 *mapptr;
    std::string message;
    std::string messageB;
    std::string lenthB;
    ub2 FileKind;
};

#endif