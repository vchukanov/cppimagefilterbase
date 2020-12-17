#pragma once

#include"img_png.h"
#include"cfg_reader.h"
#include <iostream>
#include <vector>
#include <algorithm>

class Filter
{
	config cfg;
	int getPoint(int s, int i) { return i == 0 ? 0 : s / i; }

protected:
	int getWBegin(const ImgPng& img) { return getPoint(img.getPixelData().w, cfg.left); }
	int getWEnd(const ImgPng& img) { return getPoint(img.getPixelData().w, cfg.right); }
	int getHBegin(const ImgPng& img) { return getPoint(img.getPixelData().h, cfg.up); }
	int getHEnd(const ImgPng& img) { return getPoint(img.getPixelData().h, cfg.bottom); }

public:
	void applyTo(ImgPng&);
	void setCfg(const config&);
	virtual void filtration(int, int, ImgPng&) = 0;
};

class BWFilter : public Filter
{
public:
	void filtration(int, int, ImgPng&);
};

class RedFilter : public Filter
{
public:
	void filtration(int, int, ImgPng&);
};

class Threshold : public Filter
{
private:
	unsigned char median(std::vector<unsigned char>&);
	unsigned char getThreshhold(int, int, ImgPng&);
public:
	void filtration(int, int, ImgPng&);
};

class Convolution : public Filter
{
private:
	void normirate(int&, int min, int max);
	virtual int getWeight(int i, int j) = 0;
public:
	void filtration(int, int, ImgPng&) override;
};

class Edge : public Convolution
{
private:
	const int centerWeight = 9;
	const int marginWeight = -1;
	int getWeight(int, int) override;
public:
	void applyTo(ImgPng&);
};

class Blur : public Convolution
{
	int getWeight(int, int) override;
};

class Filters
{
private:
	std::map<std::string, Filter*> filters;
	std::string filterName;
public:
	Filters();
	void setCfg(const config&);
	void applyTo(ImgPng& img);
};