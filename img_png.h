#pragma once

#include<iostream>
#include "png_toolkit.h"

class Filters;

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Pixel
{
	unsigned char* px_data_ptr;
public:
	Pixel(unsigned char*);
	void setColor(const Color&);
	Color getColor();
	unsigned char getIntensity();

};

class ImgPng : public png_toolkit
{
private:

public:
	Pixel getPixel(int x, int y);
	bool pixelIsValid(int, int);
};
