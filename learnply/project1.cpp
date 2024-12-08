#include "project1.h"
#include <algorithm>
#include <vector>
#include <cmath>

static DataStats stats;

void applyCustomSingleHue(Polyhedron* poly, icVector3& color) {
	if (!stats.isInitialized)
		initializeStats(poly);

	icVector3 M_hsv;
	RGBtoHSV(M_hsv, color);

    for (auto i = 0; i < poly->nverts; i++) {
        auto& vertex = poly->vlist[i];
        double s_v = vertex->scalar;

		// Keep hue constant, vary saturation and value
		double intensity = (s_v - stats.min) / (stats.max - stats.min);

        // Convert back to RGB
		icVector3 newHSV(M_hsv.x, intensity, intensity);
        icVector3 new_RGB;
        HSVtoRGB(newHSV, new_RGB);

        vertex->R = new_RGB.x;
        vertex->G = new_RGB.y;
        vertex->B = new_RGB.z;
    }
}
 
void applyCustomDivergent(Polyhedron* poly, icVector3& color1, icVector3& color2) {
	if (!stats.isInitialized)
		initializeStats(poly);

	// Application uses white for median color
	icVector3 medianColor = icVector3(0.91, 0.91, 0.91);

	for (int i = 0; i < poly->nverts; i++) {
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;

		double t;
		if (s_v <= stats.median) {
			if (stats.median == stats.min)
				t = 0.0;
			else
				t = (s_v - stats.min) / (stats.median - stats.min);

			t = (s_v - stats.min) / (stats.median - stats.min);
			vertex->R = color2.x * (1 - t) + medianColor.x * t;
			vertex->G = color2.y * (1 - t) + medianColor.y * t;
			vertex->B = color2.z * (1 - t) + medianColor.z * t;
		}
		else {
			if (stats.max == stats.median)
				t = 1.0;
			else
				t = (s_v - stats.median) / (stats.max - stats.median);

			vertex->R = color1.x * t + medianColor.x * (1 - t);
			vertex->G = color1.y * t + medianColor.y * (1 - t);
			vertex->B = color1.z * t + medianColor.z * (1 - t);
		}
	}
}

void applyCustomMultiHue(Polyhedron* poly, std::vector<icVector3>& colors) {
    if (!stats.isInitialized)
        initializeStats(poly);

    for (int i = 0; i < poly->nverts; i++) {
        auto& vertex = poly->vlist[i];
        double s_v = vertex->scalar;

        double pos = (1.0 - (s_v - stats.min) / (stats.max - stats.min)) * (colors.size() - 1);
        int color1Idx = pos;
        int color2Idx;

        // Handles max value while maintaining color1Idx < color2Idx
        if (color1Idx == colors.size() - 1) {
            color2Idx = color1Idx;
            color1Idx = color2Idx - 1;
        } else
            color2Idx = color1Idx + 1;

        double interp = 1 - (pos - (double)color1Idx);
		
        vertex->R = colors[color1Idx].x * interp + colors[color2Idx].x * (1 - interp);
        vertex->G = colors[color1Idx].y * interp + colors[color2Idx].y * (1 - interp);
        vertex->B = colors[color1Idx].z * interp + colors[color2Idx].z * (1 - interp);
    }
}

void applyRgbRainbow(Polyhedron* poly) {
	if (!stats.isInitialized)
        initializeStats(poly);

    for (int i = 0; i < poly->nverts; i++) {
        auto& vertex = poly->vlist[i];
        double s_v = vertex->scalar;

		double normalized = (s_v - stats.min) / (stats.max - stats.min);

		icVector3 hsv((1.0 - normalized) * 270, 1.0, 1.0);
		icVector3 rgb;
		HSVtoRGB(hsv, rgb);

		vertex->R = rgb.x;
		vertex->G = rgb.y;
		vertex->B = rgb.z;
	}
}

void applyJetRainbow(Polyhedron* poly) {
	if (!stats.isInitialized)
        initializeStats(poly);

    for (int i = 0; i < poly->nverts; i++) {
        auto& vertex = poly->vlist[i];
        double s_v = vertex->scalar;

		double normalized = (s_v - stats.min) / (stats.max - stats.min);

		double r = std::max(0.0, std::min(1.0, 1.5 - std::abs(4.0 * normalized - 3.0)));
        double g = std::max(0.0, std::min(1.0, 1.5 - std::abs(4.0 * normalized - 2.0)));
        double b = std::max(0.0, std::min(1.0, 1.5 - std::abs(4.0 * normalized - 1.0)));

        vertex->R = r;
        vertex->G = g;
        vertex->B = b;
	}
}

void applyTurboRainbow(Polyhedron* poly) {
    if (!stats.isInitialized)
        initializeStats(poly);

    const double kRedVec4[4] = { 0.13572138, 4.61539260, -42.66032258, 132.13108234 };
    const double kGreenVec4[4] = { 0.09140261, 2.19418839, 4.84296658, -14.18503333 };
    const double kBlueVec4[4] = { 0.10667330, 12.64194608, -60.58204836, 110.36276771 };
    const double kRedVec2[2] = { -152.94239396, 59.28637943 };
    const double kGreenVec2[2] = { 4.27729857, 2.82956604 };
    const double kBlueVec2[2] = { -89.90310912, 27.34824973 };

    for (int i = 0; i < poly->nverts; i++) {
        auto& vertex = poly->vlist[i];
        double s_v = vertex->scalar;

        double normalized = (s_v - stats.min) / (stats.max - stats.min);

        double v4[4] = { 1.0, normalized, normalized * normalized, normalized * normalized * normalized };
        double v2[2] = { v4[2] * v4[2], v4[2] * v4[3] };

        icVector3 rgb(0.0, 0.0, 0.0);

        for (int i = 0; i < 4; i++) {
            rgb.x += v4[i] * kRedVec4[i];
            rgb.y += v4[i] * kGreenVec4[i];
            rgb.z += v4[i] * kBlueVec4[i];
        }

        for (int i = 0; i < 2; i++) {
            rgb.x += v2[i] * kRedVec2[i];
            rgb.y += v2[i] * kGreenVec2[i];
            rgb.z += v2[i] * kBlueVec2[i];
        }
        vertex->R = rgb.x;
        vertex->G = rgb.y;
        vertex->B = rgb.z;
    }
}

double calculateLogLabLength(std::vector<icVector3>& colors) {
	// Handle Single Hue color map case
	if (colors.size() == 1) {
		return sqrt(pow(colors[0].x, 2) + pow(colors[0].y, 2) + pow(colors[0].z, 2));
	}

	std::vector<icVector3> labColors;
	labColors.reserve(colors.size());
	
	for (int i = 0; i < colors.size(); i++) {
		labColors.push_back(RGBtoLAB(colors[i]));
	}

	double labLength = 0;
	
	for (int i = 1; i < labColors.size(); i++) {
		labLength += sqrt(pow(labColors[i].x - labColors[i - 1].x, 2) +
			pow(labColors[i].y - labColors[i - 1].y, 2) +
			pow(labColors[i].z - labColors[i - 1].z, 2));
	}

	return std::log(labLength);
}

void greyscale(Polyhedron* poly) {

	double M, m;
	// find minimum and maximum scalar values
	findMm(poly, M, m);

	// traverse the vertices
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;

		// set the RGB values based on the scalar value of the vertex
		// (s_v - m)/(M-m)
		double grey = (s_v - m) / (M - m);

		// Set all RGB values to grey
		vertex->R = vertex->G = vertex->B = grey;
	}
	
}

void multi(Polyhedron* poly) {
	double M, m;
	// find minimum and maximum scalar values
	findMm(poly, M, m);

	// traverse the vertices
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;

		// pick two colors
		icVector3 c1(0.0, 1.0, 0.0);
		icVector3 c2(1.0, 0.0, 0.0);

		// set the RGB values based on the scalar value of the vertex
		double left = (s_v - m) / (M - m);
		double right = (M - s_v) / (M - m);

		icVector3 c = c1 * left + c2 * right;

		vertex->R = c.x;
		vertex->G = c.y;
		vertex->B = c.z;
	}
}

void heatmap(Polyhedron* poly) {
	double M, m;
	m = INFINITY;
	M = -m;
	findMm(poly, M, m);

	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];

		double s_v = vertex->scalar;

		// two colors
		// BLUE
		icVector3 c1(0.0, 0.0, 1.0);
		// RED
		icVector3 c2(1.0, 0.0, 0.0);
		//
		icVector3 HSVc1, HSVc2;
		RGBtoHSV(HSVc1, c1);
		RGBtoHSV(HSVc2, c2);

		double l = (s_v - m) / (M - m);
		double r = (M - s_v) / (M - m);

		icVector3 HSVc = HSVc1 * l + HSVc2 * r;
		icVector3 c;
		HSVtoRGB(HSVc, c);

		vertex->R = c.x;
		vertex->G = c.y;
		vertex->B = c.z;
	}

}

void height(Polyhedron* poly) {
	double M, m;
	// find minimum and maximum scalar values
	findMm(poly, M, m);

	// traverse the vertices
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];
		double s_v = vertex->scalar;

		// change the height of polyhedron based on the scalar value
		double l = (s_v - m) / (M - m);

		vertex->z = l * 10;
	}
}

void resetHeight(Polyhedron* poly) {
	for (auto i = 0; i < poly->nverts; i++)
		poly->vlist[i]->z = 0;
}


void RGBtoHSV(icVector3& hsv, icVector3& rgb) {

	double r = rgb.x;
	double g = rgb.y;
	double b = rgb.z;

	double& h = hsv.x;
	double& s = hsv.y;
	double& v = hsv.z;

	// find min and max of RGB respectively
	double cmax = std::max(r, std::max(g, b));
	double cmin = std::min(r, std::min(g, b));

	double diff = cmax - cmin;

	if (cmax == cmin) {
		h = 0;
	}
	else if (cmax == r)
	{
		h = fmod(60 * ((g - b) / diff) + 360, 360);
	}
	else if (cmax == g) {
		h = fmod(60 * ((b - r) / diff) + 120, 360);
	}
	else if (cmax == b) {
		h = fmod(60 * ((r - g) / diff) + 240, 360);
	}

	if (cmax == 0)
	{
		s = 0;
	}
	else {
		s = diff / cmax;
	}

	v = cmax;
}

void HSVtoRGB(icVector3& hsv, icVector3& rgb) {

	double h = hsv.x;
	double s = hsv.y;
	double v = hsv.z;

	double C = s * v;
	double X = C * (1 - abs(fmod(h / 60, 2) - 1));
	double m = v - C;

	double r, g, b;

	if (h >= 0 && h < 60)
	{
		r = C;
		g = X;
		b = 0;
	}
	else if (h >= 60 && h < 120)
	{
		r = X;
		g = C;
		b = 0;
	}
	else if (h >= 120 && h < 180)
	{
		r = 0;
		g = C;
		b = X;
	}
	else if (h >= 180 && h < 240)
	{
		r = 0;
		g = X;
		b = C;
	}
	else if (h >= 240 && h < 300)
	{
		r = X;
		g = 0;
		b = C;
	}
	else {
		r = C;
		g = 0;
		b = X;
	}

	rgb.x = (r + m);
	rgb.y = (g + m);
	rgb.z = (b + m);
}

double RGBtoXYZ(double c) {
    if (c > 0.04045) {
        return std::pow((c + 0.055) / 1.055, 2.4);
    }
    return c / 12.92;
}

double XYZtoLAB(double c) {
    if (c > 0.008856) {
        return std::pow(c, 1.0/3.0);
    }
    return (7.787 * c) + (16.0/116.0);
}

icVector3 RGBtoLAB(icVector3 rgb) {
	icVector3 lab;

	// Convert RGB to XYZ
    double r2 = RGBtoXYZ(rgb.x);
    double g2 = RGBtoXYZ(rgb.y);
    double b2 = RGBtoXYZ(rgb.z);

    double x = r2 * 0.4124564 + g2 * 0.3575761 + b2 * 0.1804375;
    double y = r2 * 0.2126729 + g2 * 0.7151522 + b2 * 0.0721750;
    double z = r2 * 0.0193339 + g2 * 0.1191920 + b2 * 0.9503041;

    // D65 reference white point
    const double xn = 0.95047;
    const double yn = 1.0;
    const double zn = 1.08883;

    // Normalize XYZ values
    x /= xn;
    y /= yn;
    z /= zn;

    // Convert XYZ to LAB
    double fx = XYZtoLAB(x);
    double fy = XYZtoLAB(y);
    double fz = XYZtoLAB(z);

    lab.x = (116.0 * fy) - 16.0;
    lab.y = 500.0 * (fx - fy);
    lab.z = 200.0 * (fy - fz);

    return lab;
}

void initializeStats(Polyhedron* poly) {
	findMm(poly, stats.max, stats.min);
	findMedian(poly, stats.median);
	stats.isInitialized = true;
}

void findMm(Polyhedron* poly, double& M, double& m) {

	// initialize for min and max
	m = INFINITY;
	M = -m;

	// traverse the vertices
	for (auto i = 0; i < poly->nverts; i++)
	{
		auto& vertex = poly->vlist[i];

		// get the min
		if (vertex->scalar < m) {
			m = vertex->scalar;
		}

		// get the max
		if (vertex->scalar > M)
		{
			M = vertex->scalar;
		}
	}
}

void findMedian(Polyhedron* poly, double& m) {
	if (poly == nullptr || poly->nverts == 0) {
		m = 0.0;
		return;
	}

	std::vector<double> values;
	values.reserve(poly->nverts);

	for (auto i = 0; i < poly->nverts; i++) {
		values.push_back(poly->vlist[i]->scalar);
	}

	std::sort(values.begin(), values.end());

	// Find median
	if (poly->nverts % 2 == 0) {
		size_t mid = poly->nverts / 2;
		m = (values[mid - 1] + values[mid]) / 2.0;
	}
	else {
		m = values[poly->nverts / 2];
	}
}
