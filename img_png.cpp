#include "img_png.h"

Pixel::Pixel(unsigned char* px_data_ptr)
{
	this->px_data_ptr = px_data_ptr;
}

void Pixel::setColor(const Color& color)
{
	px_data_ptr[0] = color.r;
	px_data_ptr[1] = color.g;
	px_data_ptr[2] = color.b;
}

Color Pixel::getColor()
{
	return Color{ px_data_ptr[0], px_data_ptr[1], px_data_ptr[2] };
}

unsigned char Pixel::getIntensity()
{
	float r_coef = 0.3f, g_coef = 0.6f, b_coef = 0.1f;
	unsigned char x = (unsigned char)(
		r_coef * (float)getColor().r +
		g_coef * (float)getColor().g +
		b_coef * (float)getColor().b);
	return x;
}

Pixel ImgPng::getPixel(int x, int y)
{
	int index = (getPixelData().w * y + x) * getPixelData().compPerPixel;
	if (index >= getPixelData().h * getPixelData().w * getPixelData().compPerPixel || index < 0)
		return NULL;
	unsigned char* px_data_ptr = &(getPixelData().pixels[index]);
	Pixel px = Pixel(px_data_ptr);
	return px;
}

bool ImgPng::pixelIsValid(int x, int y)
{
	int index = (getPixelData().w * y + x) * getPixelData().compPerPixel;
	int max_index = getPixelData().h * getPixelData().w * getPixelData().compPerPixel - 1;
	if (index >  max_index || index < 0)
		return false;
	return true;
}

