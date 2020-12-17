#include"img_filters.h"

void Filter::applyTo(ImgPng& img)
{
	for (int i = getWBegin(img); i < getWEnd(img); i++)
	{
		for (int j = getHBegin(img); j < getHEnd(img); j++)
		{
			filtration(i, j, img);
		}
	}
}

void Filter::setCfg(const config& cfg)
{
	this->cfg = cfg;
}

void BWFilter::filtration(int i, int j, ImgPng& img)
{
	unsigned char intensity = img.getPixel(i, j).getIntensity();
	Color bw_color{ intensity, intensity, intensity };
	img.getPixel(i, j).setColor(bw_color);
}

void RedFilter::filtration(int i, int j, ImgPng& img)
{
	Color red{ 255, 0, 0 };
	img.getPixel(i, j).setColor(red);
}

unsigned char Threshold::median(std::vector<unsigned char>& vec)
{
	size_t size = vec.size();
	sort(vec.begin(), vec.end());
	if (size == 0)
	{
		return 0;
	}
	else
	{
		if (size % 2 == 0)
			return (vec[size / 2 - 1] + vec[size / 2]) / 2;
		else
			return vec[size / 2];
	}
}

unsigned char Threshold::getThreshhold(int i, int j, ImgPng& img)
{
	std::vector<unsigned char> intensites;

	for (int i_shift = i - 2; i_shift <= i + 2; i_shift++)
	{
		for (int j_shift = j - 2; j_shift <= j + 2; j_shift++)
		{
			if (!img.pixelIsValid(i_shift, j_shift))
				continue;

			intensites.push_back(img.getPixel(i_shift, j_shift).getIntensity());
		}
	}

	return median(intensites);
}

void Threshold::filtration(int i, int j, ImgPng& img) 
{
	unsigned char threshold = getThreshhold(i, j, img);
	if (img.getPixel(i, j).getIntensity() < threshold)
	{
		img.getPixel(i, j).setColor(Color{ 0, 0, 0 });
	}
	else
	{
		unsigned char intensity = img.getPixel(i, j).getIntensity();
		Color bw_color{ intensity, intensity, intensity };
		img.getPixel(i, j).setColor(bw_color);
	}
}


void Convolution::normirate(int& i, int min = 0, int max = 255)
{
	if (i < min)
		i = min;
	if (i > max)
		i = max;
}

void Convolution::filtration(int i, int j, ImgPng& img)
{
	int rSum = 0, gSum = 0, bSum = 0, weightSum = 0;
	for (int iShift = i - 1; iShift <= i + 1; iShift++)
	{
		for (int jShift = j - 1; jShift <= j + 1; jShift++)
		{
			if (!img.pixelIsValid(iShift, jShift))
				continue;

			int weight = getWeight(i + 1 - iShift, j + 1 - jShift);

			Color color = img.getPixel(iShift, jShift).getColor();;

			rSum += (int)color.r * weight;
			gSum += (int)color.g * weight;
			bSum += (int)color.b * weight;
			weightSum += weight;
		}
	}

	if (weightSum <= 0)
		weightSum = 1;

	rSum /= weightSum;
	normirate(rSum);
	gSum /= weightSum;
	normirate(gSum);
	bSum /= weightSum;
	normirate(bSum);

	img.getPixel(i, j).setColor(
		Color{ 
			(unsigned char)rSum, 
			(unsigned char)gSum, 
			(unsigned char)bSum });
}

int Edge::getWeight(int i, int j)
{
	if (i == 1 && j == 1)
		return centerWeight;
	else
		return  marginWeight;	
	
}

void Edge::applyTo(ImgPng& img)
{
	BWFilter bwFilter = BWFilter();
	bwFilter.applyTo(img);

	for (int i = getWBegin(img); i < getWEnd(img); i++)
	{
		for (int j = getHBegin(img); j < getHEnd(img); j++)
		{
			filtration(i, j, img);
		}
	}
}

int Blur::getWeight(int i, int j)
{
	return 1;
}

Filters::Filters()
{
	filters.insert(std::pair<std::string, Filter*>("Red", new RedFilter()));
	filters.insert(std::pair<std::string, Filter*>("Threshold", new Threshold()));
	filters.insert(std::pair<std::string, Filter*>("Edge", new Edge()));
	filters.insert(std::pair<std::string, Filter*>("Blur", new Blur()));
	filterName = " ";
}

void Filters::setCfg(const  config& cfg)
{
	if (filters.find(cfg.filterName) != filters.end())
	{
		filterName = cfg.filterName;
		filters[cfg.filterName]->setCfg(cfg);
	}
}

void Filters::applyTo(ImgPng& img)
{
	if (filterName != " ")
		filters[filterName]->applyTo(img);
}
